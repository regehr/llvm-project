"""Shared helpers for the pattern tooling: the lossless conversion between a
pattern expression and its filename/identifier form.

A pattern is a fixed-arity DAG expression like `Add(arg0, And(arg1, arg2))`.
`expr_to_id` sanitizes it into an id (`Add_arg0_And_arg1_arg2`) that is legal
both as a filename stem and as a C++ identifier; `decode_id_to_expr` is the exact
inverse. The id is the preorder traversal of the tree with every non-alphanumeric
run collapsed to '_' -- because operator arities are known and CamelCase (no '_'
or digits) while leaves are `argN`, that token stream uniquely reconstructs the
tree, so the id is the only place the expression needs to live (no patterns.json,
no marker comment).
"""

from __future__ import annotations

import re

ARG_RE = re.compile(r"\barg(\d+)\b")
_ARG_TOKEN_RE = re.compile(r"arg\d+")
_NON_IDENT_RE = re.compile(r"[^0-9A-Za-z]+")

# Longest filename most filesystems accept (NAME_MAX). "<id>.inc" must fit.
_NAME_MAX = 255
_FILENAME_OVERHEAD = len(".inc")


def expr_arity(expression: str) -> int:
    """Number of distinct argN leaves in an expression (max index + 1)."""
    indices = [int(m.group(1)) for m in ARG_RE.finditer(expression)]
    if not indices:
        raise ValueError(f"No argN tokens in expression: {expression!r}")
    return max(indices) + 1


def expr_to_id(expression: str) -> str:
    """Turn a pattern expression into an id that is legal both as a filename
    stem and as a C++ identifier (the id is interpolated into <id>::solution,
    match<id>, Num<id>Matches... by generate_pattern_dispatcher.py).

    Every run of non-alphanumeric characters collapses to a single underscore,
    so `Add(arg0, And(arg1, arg2))` -> `Add_arg0_And_arg1_arg2`. The surviving
    op/arg tokens are the pattern's preorder traversal, which decode_id_to_expr
    inverts exactly, so the id is the only place the expression needs to live.
    Distinct patterns never collide.

    A pattern whose name would exceed NAME_MAX is rejected rather than truncated:
    truncation would make the id non-decodable. Real patterns are far short of
    the limit, so this never fires in practice.
    """
    ident = _NON_IDENT_RE.sub("_", expression).strip("_")
    if not ident:
        raise ValueError(f"expression has no identifier characters: {expression!r}")
    # A well-formed pattern's root is an operator, so the id always starts with a
    # letter -- a valid C++ identifier with no further fixup needed.
    max_stem = _NAME_MAX - _FILENAME_OVERHEAD
    if len(ident) > max_stem:
        raise ValueError(
            f"pattern too long to encode as a filename ({len(ident)} > {max_stem} "
            f"chars): {expression!r}"
        )
    return ident


def _op_arity(valid_ops, op: str) -> int:
    if valid_ops is None:
        return 2
    if not hasattr(valid_ops, "__getitem__"):
        return 2
    info = valid_ops[op]
    if isinstance(info, tuple):
        return info[0]
    return int(info)


def decode_id_to_expr(pid: str, valid_ops=None) -> str:
    """Reconstruct a pattern expression from its expression-derived id, the exact
    inverse of expr_to_id.

    The id is the underscore-joined preorder traversal of a fixed-arity tree:
    operator tokens have their declared number of children, and `argN` tokens are
    leaves. That preorder is uniquely decodable, so splitting on '_' and
    rebuilding greedily recovers the original expression text. If `valid_ops` is
    given (e.g. the dispatcher's supported-op set), every operator token is
    checked against it and its arity is used; otherwise any non-argN token is
    taken to be a binary op. Raises if the tokens don't form exactly one tree.
    """
    tokens = pid.split("_")
    pos = 0

    def build() -> str:
        nonlocal pos
        if pos >= len(tokens):
            raise ValueError(f"truncated pattern id (ran out of tokens): {pid!r}")
        tok = tokens[pos]
        pos += 1
        if _ARG_TOKEN_RE.fullmatch(tok):
            return tok
        if valid_ops is not None and tok not in valid_ops:
            raise ValueError(f"unknown op token {tok!r} in pattern id {pid!r}")
        args = [build() for _ in range(_op_arity(valid_ops, tok))]
        return f"{tok}({', '.join(args)})"

    expr = build()
    if pos != len(tokens):
        raise ValueError(f"trailing tokens in pattern id {pid!r}")
    return expr
