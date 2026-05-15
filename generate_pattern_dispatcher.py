from argparse import ArgumentParser
from dataclasses import dataclass
from json import loads
from pathlib import Path
from collections import defaultdict
import re


@dataclass(frozen=True)
class Arg:
    idx: int


@dataclass(frozen=True)
class Op:
    name: str
    lhs: "Node"
    rhs: "Node"


Node = Arg | Op


@dataclass(frozen=True)
class PatternSpec:
    id: str
    expr: str
    ast: Op
    root: str
    max_arg: int


NSW = "OverflowingBinaryOperator::NoSignedWrap"
NUW = "OverflowingBinaryOperator::NoUnsignedWrap"
NSWNUW = f"({NSW} | {NUW})"

OP_MATCHER = {
    "Add": "m_c_ExactWrapAdd<0>",
    "AddNsw": f"m_c_ExactWrapAdd<{NSW}>",
    "AddNuw": f"m_c_ExactWrapAdd<{NUW}>",
    "AddNswNuw": "m_c_NSWNUWAdd",
    "And": "m_c_And",
    "Ashr": "m_AShr",
    "AshrExact": "m_Exact(m_AShr({L}, {R}))",
    "Lshr": "m_LShr",
    "LshrExact": "m_Exact(m_LShr({L}, {R}))",
    "Mul": "m_c_ExactWrapMul<0>",
    "MulNsw": f"m_c_ExactWrapMul<{NSW}>",
    "MulNuw": f"m_c_ExactWrapMul<{NUW}>",
    "MulNswNuw": "m_c_NSWNUWMul",
    "Or": "m_c_Or",
    "OrDisjoint": "m_c_DisjointOr",
    "Sdiv": "m_SDiv",
    "SdivExact": "m_Exact(m_SDiv({L}, {R}))",
    "Shl": "m_ExactWrapShl<0>",
    "ShlNsw": f"m_ExactWrapShl<{NSW}>",
    "ShlNuw": f"m_ExactWrapShl<{NUW}>",
    "ShlNswNuw": f"m_ExactWrapShl<{NSWNUW}>",
    "Srem": "m_SRem",
    "Sub": "m_ExactWrapSub<0>",
    "SubNsw": f"m_ExactWrapSub<{NSW}>",
    "SubNuw": f"m_ExactWrapSub<{NUW}>",
    "SubNswNuw": f"m_ExactWrapSub<{NSWNUW}>",
    "Udiv": "m_UDiv",
    "UdivExact": "m_Exact(m_UDiv({L}, {R}))",
    "Urem": "m_URem",
    "Xor": "m_c_Xor",
}


def parse_expr(text: str) -> Node:
    text = text.strip()
    m = re.fullmatch(r"arg([0-9]+)", text)
    if m:
        return Arg(int(m.group(1)))

    lpar = text.find("(")
    if lpar < 0 or not text.endswith(")"):
        raise ValueError(f"Invalid expression: {text}")

    op = text[:lpar]
    inner = text[lpar + 1 : -1]
    depth = 0
    split = -1
    for i, c in enumerate(inner):
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
        elif c == "," and depth == 0:
            if i + 1 >= len(inner) or inner[i + 1] != " ":
                raise ValueError(f"Expected ', ' separator in: {text}")
            split = i
            break
    if split < 0:
        raise ValueError(f"Binary expression expected: {text}")

    lhs = inner[:split]
    rhs = inner[split + 2 :]
    return Op(op, parse_expr(lhs), parse_expr(rhs))


def max_arg(node: Node) -> int:
    if isinstance(node, Arg):
        return node.idx
    return max(max_arg(node.lhs), max_arg(node.rhs))


def split_top_level_args(s: str) -> list[str]:
    args: list[str] = []
    depth_paren = 0
    depth_angle = 0
    start = 0
    for i, ch in enumerate(s):
        if ch == "(":
            depth_paren += 1
        elif ch == ")":
            depth_paren -= 1
        elif ch == "<":
            depth_angle += 1
        elif ch == ">":
            depth_angle -= 1
        elif ch == "," and depth_paren == 0 and depth_angle == 0:
            args.append(s[start:i].strip())
            start = i + 1
    args.append(s[start:].strip())
    return args


def parse_call(expr: str) -> tuple[str, list[str]] | None:
    expr = expr.strip()
    if not expr.endswith(")"):
        return None
    lpar = expr.find("(")
    if lpar < 0:
        return None
    callee = expr[:lpar].strip()
    inner = expr[lpar + 1 : -1]
    depth = 0
    for ch in inner:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth < 0:
                return None
    if depth != 0:
        return None
    return callee, split_top_level_args(inner)


def format_matcher(expr: str, indent: int, width: int) -> list[str]:
    one_line = (" " * indent) + expr
    if len(one_line) <= width:
        return [one_line]

    parsed = parse_call(expr)
    if not parsed:
        return [one_line]
    callee, args = parsed
    if not args:
        return [(" " * indent) + f"{callee}()"]

    out = [(" " * indent) + f"{callee}("]
    for i, arg in enumerate(args):
        arg_lines = format_matcher(arg, indent + 2, width)
        if i != len(args) - 1:
            arg_lines[-1] += ","
        out.extend(arg_lines)
    out.append((" " * indent) + ")")
    return out


def emit_guard(matcher: str) -> str:
    one_line = f"  if (!match(I, {matcher})) return std::nullopt;"
    if len(one_line) <= 100:
        return one_line + "\n"

    matcher_lines = format_matcher(matcher, indent=0, width=84)
    if len(matcher_lines) == 1:
        return f"  if (!match(I, {matcher_lines[0]}))\n    return std::nullopt;\n"
    hang = " " * len("  if (!match(I, ")
    out: list[str] = []
    out.append(f"  if (!match(I, {matcher_lines[0].strip()}\n")
    for line in matcher_lines[1:-1]:
        out.append(f"{hang}{line}\n")
    out.append(f"{hang}{matcher_lines[-1]}))\n")
    out.append("    return std::nullopt;\n")
    return "".join(out)


def compose_op_matcher(op_name: str, lhs: str, rhs: str) -> str:
    pattern = OP_MATCHER.get(op_name)
    if pattern is None:
        raise ValueError(f"Unsupported op in expression: {op_name}")
    if "{L}" in pattern:
        return pattern.format(L=lhs, R=rhs)
    return f"{pattern}({lhs}, {rhs})"


def emit_match_expr(node: Node, seen_args: set[int]) -> str:
    if isinstance(node, Arg):
        if node.idx in seen_args:
            return f"m_Deferred(Arg{node.idx})"
        seen_args.add(node.idx)
        return f"m_Value(Arg{node.idx})"

    lhs = emit_match_expr(node.lhs, seen_args)
    rhs = emit_match_expr(node.rhs, seen_args)

    return compose_op_matcher(node.name, lhs, rhs)


def emit_pattern_function(spec: PatternSpec) -> str:
    r = range(spec.max_arg + 1)
    arg_decl = "const Value " + ", ".join(f"*Arg{i} = nullptr" for i in r) + ";"

    matcher = emit_match_expr(spec.ast, set())
    out: list[str] = []
    out.append(
        f"static std::optional<KnownBits> match{spec.id}(const Operator *I, const SimplifyQuery &Q, unsigned Depth) {{\n"
    )
    out.append(f"  {arg_decl}\n")
    out.append(emit_guard(matcher))
    out.append(f"  ++NumPattern{spec.id}Matches;\n")
    out.append(
        f'  LLVM_DEBUG(dbgs() << "[KnownBits DAG] matched pattern {spec.id} on: " << *I << "\\n");\n'
    )
    for i in r:
        out.append(f"  auto ArrArg{i} = kbToArr(computeKnownBits(Arg{i}, Q, Depth));\n")
    arg_list = ", ".join(f"ArrArg{i}" for i in r)
    out.append(f"  return arrToKB(Pattern{spec.id}::solution({arg_list}));\n")
    out.append("}\n")
    return "".join(out)


def emit_dispatch(roots: dict[str, list[PatternSpec]]) -> str:
    out: list[str] = []
    out.append(
        "static std::optional<KnownBits> computePatternKB(const Operator *I, const SimplifyQuery &Q, unsigned Depth) {\n"
    )
    out.append("  std::optional<KnownBits> Acc;\n")
    out.append("  switch (classifyPatternOp(I, Q)) {\n")
    for root in sorted(roots.keys()):
        out.append(f"  case PatternOp::{root}:\n")
        for spec in roots[root]:
            out.append(f"    mergePatternKB(Acc, match{spec.id}(I, Q, Depth + 1));\n")
        out.append("    break;\n")
    out.append("  default:\n")
    out.append("    break;\n")
    out.append("  }\n")
    out.append("  return Acc;\n")
    out.append("}\n")
    return "".join(out)


def main() -> None:
    ap = ArgumentParser()
    ap.add_argument("-i", "--input", type=Path, required=True)
    ap.add_argument("-o", "--output", type=Path, required=True)
    args = ap.parse_args()

    data = loads(args.input.read_text())
    specs: list[PatternSpec] = []
    for pid, expr in data.items():
        ast = parse_expr(expr)
        assert isinstance(ast, Op)
        specs.append(
            PatternSpec(id=pid, expr=expr, ast=ast, root=ast.name, max_arg=max_arg(ast))
        )
    specs.sort(key=lambda s: s.id)

    unknown_ops = sorted({s.root for s in specs if s.root not in OP_MATCHER})
    if unknown_ops:
        raise ValueError(f"patterns.json contains unmapped ops: {unknown_ops}")

    roots: dict[str, list[PatternSpec]] = defaultdict(list)
    for spec in specs:
        roots[spec.root].append(spec)

    out: list[str] = []
    out.append("// Auto-generated by generate_pattern_dispatcher.py. Do not edit.\n")
    for spec in specs:
        out.append(f'#include "patterns/pattern_{spec.id}.inc" // {spec.expr}\n')
    out.append("\n")
    for spec in specs:
        out.append(
            f'ALWAYS_ENABLED_STATISTIC(NumPattern{spec.id}Matches, "KnownBits DAG pattern {spec.id} matches");\n'
        )
    out.append("\n")

    for spec in specs:
        out.append(emit_pattern_function(spec))
        out.append("\n")

    out.append(emit_dispatch(roots))

    args.output.write_text("".join(out))


if __name__ == "__main__":
    main()
