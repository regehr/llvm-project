from argparse import ArgumentParser
from collections import defaultdict
from dataclasses import dataclass
from json import loads
from pathlib import Path
import re
from typing import Callable, Iterable


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


COMMUTATIVE_OPS = {
    "Add",
    "AddNsw",
    "AddNuw",
    "AddNswNuw",
    "And",
    "Mul",
    "MulNsw",
    "MulNuw",
    "MulNswNuw",
    "Or",
    "OrDisjoint",
    "Xor",
}

RET_NULL = "return std::nullopt;"
RET_FALSE = "return false;"


class CodeWriter:
    def __init__(self) -> None:
        self._lines: list[str] = []

    def line(self, text: str = "", indent: int = 0) -> None:
        self._lines.append((" " * indent) + text + "\n")

    def lines(self, lines: Iterable[str], indent: int = 0) -> None:
        for line in lines:
            self.line(line, indent)

    def text(self) -> str:
        return "".join(self._lines)


def compact_if_return_lines(text: str) -> str:
    lines = text.splitlines()
    out: list[str] = []
    i = 0
    while i < len(lines):
        cur = lines[i]
        if i + 1 < len(lines):
            nxt = lines[i + 1]
            m_if = re.fullmatch(r"(\s*)if \((.+)\)", cur)
            m_ret = re.fullmatch(r"\s*(return .+;)", nxt)
            if m_if and m_ret:
                indent = m_if.group(1)
                cond = m_if.group(2)
                ret = m_ret.group(1)
                out.append(f"{indent}if ({cond}) {ret}")
                i += 2
                continue
        out.append(cur)
        i += 1
    return "\n".join(out) + "\n"


def align_if_return_lines(text: str) -> str:
    lines = text.splitlines()
    out: list[str] = []
    i = 0
    pat = re.compile(r"(\s*if \(.+?\)) (return .+;)")
    while i < len(lines):
        m = pat.fullmatch(lines[i])
        if not m:
            out.append(lines[i])
            i += 1
            continue
        block = []
        j = i
        while j < len(lines):
            m2 = pat.fullmatch(lines[j])
            if not m2:
                break
            block.append((m2.group(1), m2.group(2)))
            j += 1
        width = max(len(prefix) for prefix, _ in block)
        for prefix, ret in block:
            out.append(prefix.ljust(width) + " " + ret)
        i = j
    return "\n".join(out) + "\n"


def format_output(text: str) -> str:
    return align_if_return_lines(compact_if_return_lines(text))


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


def root_gate_expr(
    left_expr: str, left_node: Node, right_expr: str, right_node: Node
) -> str:
    left_is_op = isinstance(left_node, Op)
    right_is_op = isinstance(right_node, Op)
    if left_is_op and right_is_op:
        assert isinstance(left_node, Op) and isinstance(right_node, Op)
        return (
            f"(hasPatternOp({left_expr}, Q, PatternOp::{left_node.name})) && "
            f"(hasPatternOp({right_expr}, Q, PatternOp::{right_node.name}))"
        )
    if left_is_op:
        assert isinstance(left_node, Op)
        return f"hasPatternOp({left_expr}, Q, PatternOp::{left_node.name})"
    if right_is_op:
        assert isinstance(right_node, Op)
        return f"hasPatternOp({right_expr}, Q, PatternOp::{right_node.name})"
    return "true"


def op_pred(expr: str, op_name: str) -> str:
    return f"hasPatternOp({expr}, Q, PatternOp::{op_name})"


def is_symmetric_root_gate(ast: Op, root_fwd: str, root_rev: str) -> bool:
    return (
        isinstance(ast.lhs, Op)
        and isinstance(ast.rhs, Op)
        and ast.lhs.name == ast.rhs.name
    ) or (root_fwd == root_rev)


class PatternEmitter:
    def __init__(self, max_arg_idx: int, pattern_id: str, pattern_expr: str):
        self.max_arg_idx = max_arg_idx
        self.pattern_id = pattern_id
        self.pattern_expr = pattern_expr
        self.tmp_idx = 0

    def tmp(self, prefix: str) -> str:
        name = f"{prefix}{self.tmp_idx}"
        self.tmp_idx += 1
        return name

    def depth(self, node: Node) -> int:
        if isinstance(node, Arg):
            return 0
        return 1 + self.depth(node.lhs) + self.depth(node.rhs)

    def emit_if_fail(
        self, w: CodeWriter, indent: int, cond: str, fail_stmt: str
    ) -> None:
        w.line(f"if (!{cond})", indent)
        w.line(f"  {fail_stmt}", indent)

    def emit_op_vars(
        self, w: CodeWriter, indent: int, expr: str, op_name: str, prefix: str = "K"
    ) -> tuple[str, str]:
        v = self.tmp(prefix)
        l = f"{v}L"
        r = f"{v}R"
        self.emit_known_node_operands(w, indent, expr, op_name, l, r, None)
        return l, r

    def emit_children(
        self,
        op_name: str,
        lhs: Node,
        rhs: Node,
        lhs_expr: str,
        rhs_expr: str,
        w: CodeWriter,
        indent: int,
        fail_stmt: str,
    ) -> None:
        if op_name in COMMUTATIVE_OPS:
            self.emit_match_comm(lhs, rhs, lhs_expr, rhs_expr, w, indent, fail_stmt)
        else:
            self.emit_match_order(lhs, rhs, lhs_expr, rhs_expr, w, indent, fail_stmt)

    def emit_known_node_operands(
        self,
        w: CodeWriter,
        indent: int,
        value_expr: str,
        op_name: str,
        lhs_var: str,
        rhs_var: str,
        fail_stmt: str | None,
    ) -> None:
        w.line(f"const Value *{lhs_var} = nullptr, *{rhs_var} = nullptr;", indent)
        call = f"matchPatternNode({value_expr}, Q, PatternOp::{op_name}, {lhs_var}, {rhs_var})"
        if fail_stmt is None:
            w.line(f"(void){call};", indent)
        else:
            self.emit_if_fail(w, indent, call, fail_stmt)

    # Match a single node (arg or op) against a value expression. Example: Add(arg0, arg1).
    def emit_match_node(
        self, node: Node, value_expr: str, w: CodeWriter, indent: int, fail_stmt: str
    ) -> None:
        if isinstance(node, Arg):
            self.emit_if_fail(
                w, indent, f"bindPatternArg(Arg{node.idx}, {value_expr})", fail_stmt
            )
            return

        op_var = self.tmp("N")
        lhs_var = f"{op_var}L"
        rhs_var = f"{op_var}R"
        self.emit_known_node_operands(
            w, indent, value_expr, node.name, lhs_var, rhs_var, fail_stmt
        )

        self.emit_children(
            node.name, node.lhs, node.rhs, lhs_var, rhs_var, w, indent, fail_stmt
        )

    def emit_match_known_op_children(
        self,
        node: Op,
        lhs_expr: str,
        rhs_expr: str,
        w: CodeWriter,
        indent: int,
        fail_stmt: str,
    ) -> None:
        self.emit_children(
            node.name, node.lhs, node.rhs, lhs_expr, rhs_expr, w, indent, fail_stmt
        )

    # Match ordered children; check deeper side first. Example: Shl(Add(arg1, arg2), arg0).
    def emit_match_order(
        self,
        lhs: Node,
        rhs: Node,
        lhs_expr: str,
        rhs_expr: str,
        w: CodeWriter,
        indent: int,
        fail_stmt: str,
    ) -> None:
        first_n, first_e, second_n, second_e = lhs, lhs_expr, rhs, rhs_expr
        if self.depth(rhs) > self.depth(lhs):
            first_n, first_e, second_n, second_e = rhs, rhs_expr, lhs, lhs_expr
        self.emit_match_node(first_n, first_e, w, indent, fail_stmt)
        self.emit_match_node(second_n, second_e, w, indent, fail_stmt)

    def emit_orient2(
        self,
        w: CodeWriter,
        indent: int,
        cond_fwd: str,
        emit_fwd: Callable[[int], None],
        cond_rev: str,
        emit_rev: Callable[[int], None],
        fail_stmt: str,
    ) -> None:
        w.line(f"if ({cond_fwd}) {{", indent)
        emit_fwd(indent + 2)
        w.line(f"}} else if ({cond_rev}) {{", indent)
        emit_rev(indent + 2)
        w.line("} else {", indent)
        w.line(f"{fail_stmt}", indent + 2)
        w.line("}", indent)

    # Match commutative children with orientation checks. Example: AddNswNuw(arg0, AddNswNuw(arg1, arg2)).
    def emit_match_comm(
        self,
        lhs: Node,
        rhs: Node,
        lhs_expr: str,
        rhs_expr: str,
        w: CodeWriter,
        indent: int,
        fail_stmt: str,
    ) -> None:
        lhs_op = lhs if isinstance(lhs, Op) else None
        rhs_op = rhs if isinstance(rhs, Op) else None

        if (lhs_op and not rhs_op) or (rhs_op and not lhs_op):
            op_node = lhs if lhs_op else rhs
            nonop_node = rhs if lhs_op else lhs
            op_expr = lhs_expr if lhs_op else rhs_expr
            alt_expr = rhs_expr if lhs_op else lhs_expr
            assert isinstance(op_node, Op)

            def emit_fwd(n: int) -> None:
                op_l, op_r = self.emit_op_vars(w, n, op_expr, op_node.name)
                self.emit_match_known_op_children(
                    op_node, op_l, op_r, w, n, fail_stmt
                )
                self.emit_match_node(nonop_node, alt_expr, w, n, fail_stmt)

            def emit_rev(n: int) -> None:
                op_l, op_r = self.emit_op_vars(w, n, alt_expr, op_node.name)
                self.emit_match_known_op_children(
                    op_node, op_l, op_r, w, n, fail_stmt
                )
                self.emit_match_node(nonop_node, op_expr, w, n, fail_stmt)

            self.emit_orient2(
                w,
                indent,
                op_pred(op_expr, op_node.name),
                emit_fwd,
                op_pred(alt_expr, op_node.name),
                emit_rev,
                fail_stmt,
            )
            return

        if lhs_op and rhs_op and lhs_op.name != rhs_op.name:

            def emit_fwd(n: int) -> None:
                lhs_l, lhs_r = self.emit_op_vars(w, n, lhs_expr, lhs_op.name)
                rhs_l, rhs_r = self.emit_op_vars(w, n, rhs_expr, rhs_op.name)
                self.emit_match_known_op_children(
                    lhs_op, lhs_l, lhs_r, w, n, fail_stmt
                )
                self.emit_match_known_op_children(
                    rhs_op, rhs_l, rhs_r, w, n, fail_stmt
                )

            def emit_rev(n: int) -> None:
                lhs_l, lhs_r = self.emit_op_vars(w, n, lhs_expr, rhs_op.name)
                rhs_l, rhs_r = self.emit_op_vars(w, n, rhs_expr, lhs_op.name)
                self.emit_match_known_op_children(
                    lhs_op, rhs_l, rhs_r, w, n, fail_stmt
                )
                self.emit_match_known_op_children(
                    rhs_op, lhs_l, lhs_r, w, n, fail_stmt
                )

            self.emit_orient2(
                w,
                indent,
                f"({op_pred(lhs_expr, lhs_op.name)}) && ({op_pred(rhs_expr, rhs_op.name)})",
                emit_fwd,
                f"({op_pred(lhs_expr, rhs_op.name)}) && ({op_pred(rhs_expr, lhs_op.name)})",
                emit_rev,
                fail_stmt,
            )
            return

        if isinstance(lhs, Arg) and isinstance(rhs, Arg):
            self.emit_if_fail(
                w,
                indent,
                f"bindPatternPairCommutative({lhs_expr}, {rhs_expr}, Arg{lhs.idx}, Arg{rhs.idx})",
                fail_stmt,
            )
            return

        raise ValueError(
            f"Pattern {self.pattern_id} ({self.pattern_expr}) has unsupported nested "
            f"commutative shape requiring rollback: lhs={lhs!r}, rhs={rhs!r}"
        )


# Top-Level Emission
def emit_solution_tail(w: CodeWriter, spec: PatternSpec, r: range) -> None:
    w.line(
        f'LLVM_DEBUG(dbgs() << "[KnownBits DAG] matched pattern {spec.id} on: " << *I << "\\n");',
        2,
    )
    w.lines(
        (f"auto ArrArg{i} = kbToArr(computeKnownBits(Arg{i}, Q, Depth));" for i in r),
        2,
    )
    arg_list = ", ".join(f"ArrArg{i}" for i in r)
    w.line(f"return arrToKB(Pattern{spec.id}::solution({arg_list}));", 2)


def emit_pattern_function(spec: PatternSpec) -> str:
    w = CodeWriter()
    id = spec.id
    ast = spec.ast
    r = range(spec.max_arg + 1)

    w.line(
        f"static std::optional<KnownBits> match{id}(const Operator *I, const SimplifyQuery &Q, unsigned Depth) {{"
    )
    w.line("const Value " + ", ".join(f"*Arg{i} = nullptr" for i in r) + ";", 2)
    w.line("const Value *RootL = I->getOperand(0), *RootR = I->getOperand(1);", 2)

    emitter = PatternEmitter(spec.max_arg, spec.id, spec.expr)
    if ast.name in COMMUTATIVE_OPS:
        w.line("auto matchRootOrder = [&](const Value *A, const Value *B) -> bool {", 2)
        emitter.emit_match_order(ast.lhs, ast.rhs, "A", "B", w, 4, RET_FALSE)
        w.line("return true;", 4)
        w.line("};", 2)

        root_fwd = root_gate_expr("RootL", ast.lhs, "RootR", ast.rhs)
        root_rev = root_gate_expr("RootR", ast.lhs, "RootL", ast.rhs)
        same_root_gate = is_symmetric_root_gate(ast, root_fwd, root_rev)

        w.line(f"const bool CanMatchFwd = {root_fwd};", 2)
        if same_root_gate:
            w.line("const bool CanMatchRev = CanMatchFwd;", 2)
        else:
            w.line(f"const bool CanMatchRev = {root_rev};", 2)

        w.line(
            "const bool Matched = matchEitherRootOrder("
            "CanMatchFwd, CanMatchRev, RootL, RootR, matchRootOrder, "
            + ", ".join(f"Arg{i}" for i in r)
            + ");",
            2,
        )
        w.line("if (!Matched)", 2)
        w.line(RET_NULL, 4)
    else:
        emitter.emit_match_order(ast.lhs, ast.rhs, "RootL", "RootR", w, 2, RET_NULL)

    emit_solution_tail(w, spec, r)
    w.line("}")
    return w.text()


def emit_dispatch(roots: dict[str, list[PatternSpec]]) -> str:
    w = CodeWriter()
    w.line(
        "static std::optional<KnownBits> computePatternKB(const Operator *I, const SimplifyQuery &Q, unsigned Depth) {"
    )
    w.line("std::optional<KnownBits> Acc;", 2)
    w.line("switch (classifyPatternOp(I, Q)) {", 2)
    for root in sorted(roots.keys()):
        w.line(f"case PatternOp::{root}:", 2)
        for spec in roots[root]:
            w.line(f"mergePatternKB(Acc, match{spec.id}(I, Q, Depth + 1));", 4)
        w.line("break;", 4)
    w.line("default:", 2)
    w.line("break;", 4)
    w.line("}", 2)
    w.line("return Acc;", 2)
    w.line("}")
    return w.text()


def main() -> None:
    ap = ArgumentParser()
    ap.add_argument("-i", "--input", type=Path, required=True)
    ap.add_argument("-o", "--output", type=Path, required=True)
    args = ap.parse_args()

    data = loads(args.input.read_text())
    specs: list[PatternSpec] = []
    for id, expr in data.items():
        ast = parse_expr(expr)
        assert isinstance(ast, Op)
        specs.append(
            PatternSpec(id=id, expr=expr, ast=ast, root=ast.name, max_arg=max_arg(ast))
        )
    specs.sort(key=lambda s: s.id)

    roots: dict[str, list[PatternSpec]] = defaultdict(list)
    for spec in specs:
        roots[spec.root].append(spec)

    w = CodeWriter()
    w.line("// Auto-generated by generate_pattern_dispatcher.py. Do not edit.")
    for spec in specs:
        w.line(f'#include "patterns/pattern_{spec.id}.inc" // {spec.expr}')
    w.line()

    for spec in specs:
        w.lines(emit_pattern_function(spec).splitlines())
        w.line()

    w.lines(emit_dispatch(roots).splitlines())
    args.output.write_text(format_output(w.text()))


if __name__ == "__main__":
    main()
