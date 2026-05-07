from argparse import ArgumentParser
from json import loads
import re
import textwrap
from dataclasses import dataclass
from pathlib import Path


@dataclass
class Arg:
    idx: int


@dataclass
class Op:
    name: str
    lhs: "Op | Arg"
    rhs: "Op | Arg"


def parse_expr(text: str) -> Arg | Op:
    text = text.strip()
    m = re.compile(r"arg([0-9]+)$").fullmatch(text)
    if m:
        return Arg(int(m.group(1)))

    lpar = text.find("(")
    if lpar == -1 or not text.endswith(")"):
        raise ValueError(f"Invalid expression: {text}")

    op = text[:lpar]
    inner = text[lpar + 1 : -1]
    depth = 0
    split = -1
    for i in range(len(inner)):
        c = inner[i]
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
        elif c == "," and depth == 0:
            if i + 1 >= len(inner) or inner[i + 1] != " ":
                raise ValueError(f"Expected ', ' separator in: {text}")
            split = i
            break
    if split == -1:
        raise ValueError(f"Binary expression expected: {text}")
    lhs = inner[:split]
    rhs = inner[split + 2 :]
    return Op(op, parse_expr(lhs), parse_expr(rhs))


def emit_match(node: Arg | Op, arg_vars) -> str:
    if isinstance(node, Arg):
        var = f"Arg{node.idx}"
        if var not in arg_vars:
            arg_vars.add(var)
            return f"m_Value({var})"
        return f"m_Deferred({var})"

    op = node.name
    lhs = emit_match(node.lhs, arg_vars)
    rhs = emit_match(node.rhs, arg_vars)
    if op == "Add":
        return f"m_c_Add({lhs}, {rhs})"
    if op == "AddNsw":
        return f"m_c_NSWAdd({lhs}, {rhs})"
    if op == "AddNuw":
        return f"m_c_NUWAdd({lhs}, {rhs})"
    if op == "AddNswNuw":
        return (
            "m_CombineOr("
            f"m_CombineAnd(m_NSWAdd({lhs}, {rhs}), m_NUWAdd({lhs}, {rhs})), "
            f"m_CombineAnd(m_NSWAdd({rhs}, {lhs}), m_NUWAdd({rhs}, {lhs})))"
        )
    if op == "And":
        return f"m_c_And({lhs}, {rhs})"
    if op == "Ashr":
        return f"m_AShr({lhs}, {rhs})"
    if op == "AshrExact":
        return f"m_Exact(m_AShr({lhs}, {rhs}))"
    if op == "Lshr":
        return f"m_LShr({lhs}, {rhs})"
    if op == "LshrExact":
        return f"m_Exact(m_LShr({lhs}, {rhs}))"
    if op == "Mul":
        return f"m_c_Mul({lhs}, {rhs})"
    if op == "MulNsw":
        return f"m_CombineOr(m_NSWMul({lhs}, {rhs}), m_NSWMul({rhs}, {lhs}))"
    if op == "MulNuw":
        return f"m_CombineOr(m_NUWMul({lhs}, {rhs}), m_NUWMul({rhs}, {lhs}))"
    if op == "MulNswNuw":
        return (
            "m_CombineOr("
            f"m_CombineAnd(m_NSWMul({lhs}, {rhs}), m_NUWMul({lhs}, {rhs})), "
            f"m_CombineAnd(m_NSWMul({rhs}, {lhs}), m_NUWMul({rhs}, {lhs})))"
        )
    if op == "Or":
        return f"m_c_Or({lhs}, {rhs})"
    if op == "OrDisjoint":
        return f"m_c_DisjointOr({lhs}, {rhs})"
    if op == "Sdiv":
        return f"m_SDiv({lhs}, {rhs})"
    if op == "SdivExact":
        return f"m_Exact(m_SDiv({lhs}, {rhs}))"
    if op == "Shl":
        return f"m_Shl({lhs}, {rhs})"
    if op == "ShlNsw":
        return f"m_NSWShl({lhs}, {rhs})"
    if op == "ShlNuw":
        return f"m_NUWShl({lhs}, {rhs})"
    if op == "ShlNswNuw":
        return f"m_CombineAnd(m_NSWShl({lhs}, {rhs}), m_NUWShl({lhs}, {rhs}))"
    if op == "Srem":
        return f"m_SRem({lhs}, {rhs})"
    if op == "Sub":
        return f"m_Sub({lhs}, {rhs})"
    if op == "SubNsw":
        return f"m_NSWSub({lhs}, {rhs})"
    if op == "SubNuw":
        return f"m_NUWSub({lhs}, {rhs})"
    if op == "SubNswNuw":
        return f"m_CombineAnd(m_NSWSub({lhs}, {rhs}), m_NUWSub({lhs}, {rhs}))"
    if op == "Udiv":
        return f"m_UDiv({lhs}, {rhs})"
    if op == "UdivExact":
        return f"m_Exact(m_UDiv({lhs}, {rhs}))"
    if op == "Urem":
        return f"m_URem({lhs}, {rhs})"
    if op == "Xor":
        return f"m_c_Xor({lhs}, {rhs})"

    raise ValueError(f"Unsupported op: {op}")


def max_arg(node: Arg | Op) -> int:
    if isinstance(node, Arg):
        return node.idx
    return max(max_arg(node.lhs), max_arg(node.rhs))


def emit_wrapped_match_guard(matcher: str) -> list[str]:
    expr = f"if (!match(I, {matcher}))"
    lines = textwrap.wrap(
        expr,
        width=96,
        initial_indent="  ",
        subsequent_indent="          ",
        break_long_words=False,
        break_on_hyphens=False,
    )
    return [line + "\n" for line in lines]


merge_fn = """
  std::optional<KnownBits> Acc;
  auto Merge = [&](const std::optional<KnownBits> &R) {
    if (!R)
      return;
    if (!Acc)
      Acc = *R;
    else
      Acc = Acc->intersectWith(*R);
  };
"""


def main():
    ap = ArgumentParser()
    ap.add_argument("-i", "--input", type=Path)
    ap.add_argument("-o", "--output", type=Path)
    args = ap.parse_args()

    data = loads(args.input.read_text())
    items: list[tuple[str, str, Op, str, int]] = []
    for pid, expr in data.items():
        ast = parse_expr(expr)
        assert isinstance(ast, Op)
        items.append((pid, expr, ast, ast.name, max_arg(ast)))
    items.sort(key=lambda x: x[0])

    by_root = {}
    for it in items:
        by_root.setdefault(it[3], []).append(it)

    out: list[str] = []
    out.append("// Auto-generated by generate_pattern_dispatcher.py. Do not edit.\n")
    out.extend(
        [
            f'#include "patterns/pattern_{x}.inc" // {data[x]}\n'
            for x, _, _, _, _ in items
        ]
    )
    out.append("\n")

    for pid, _, ast, _, m_arg in items:
        arg_vars = set()
        matcher = emit_match(ast, arg_vars)
        out.append(f"static std::optional<KnownBits> matchPattern{pid}(\n")
        out.append("    const Operator *I, const SimplifyQuery &Q, unsigned Depth) {\n")
        out.extend([f"  const Value *Arg{i} = nullptr;\n" for i in range(m_arg + 1)])
        out.extend(emit_wrapped_match_guard(matcher))
        out.append("    return std::nullopt;\n")
        out.append(
            f'  LLVM_DEBUG(dbgs() << "[KnownBits DAG] matched pattern {pid} on: "'
            ' << *I << "\\n");\n'
        )
        for i in range(m_arg + 1):
            out.append(
                f"  KnownBits KBArg{i} = computeKnownBits(Arg{i}, Q, Depth + 1);\n"
            )
            out.append(f"  auto ArrArg{i} = knownBitsToArray(KBArg{i});\n")
        arg_list = ", ".join([f"ArrArg{i}" for i in range(m_arg + 1)])
        out.append(f"  auto Out = Pattern{pid}::solution({arg_list});\n")
        out.append("  return arrayToKnownBits(Out);\n")
        out.append("}\n\n")

    out.append("static std::optional<KnownBits> computePatternKB(\n")
    out.append("    const Operator *I, const SimplifyQuery &Q, unsigned Depth) {\n")
    out.append(merge_fn)
    out.append("  switch (getKBPatternRoot(I, Q)) {\n")
    for root in sorted(by_root.keys()):
        out.append(f"  case KBPatternRoot::{root}:\n")
        for pid, _, _, _, _ in by_root[root]:
            out.append(f"    Merge(matchPattern{pid}(I, Q, Depth));\n")
        out.append("    break;\n")
    out.append("  default:\n    break;\n")
    out.append("  }\n")
    out.append("  return Acc;\n")
    out.append("}\n")

    Path(args.output).write_text("".join(out))


if __name__ == "__main__":
    main()
