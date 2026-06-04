#!/usr/bin/env python3
"""Generate top-returning <id>::solution .inc files for a set of patterns.

Each pattern is named after its own expression: id = expr_to_id(expression), so
`Add(arg0, And(arg1, arg2))` becomes the file Add_arg0_And_arg1_arg2.inc holding
`namespace Add_arg0_And_arg1_arg2`. The sanitized id is a lossless preorder
encoding of the (all-binary) pattern tree, so generate_pattern_dispatcher.py
recovers the expression by decoding the filename alone -- no marker comment and
no patterns.json index.

Input is a TSV (--tsv) with a header row and a `pattern` column holding the
expression string directly, e.g. results/top_1000_pattern.tsv:

    count   size   pattern
    6359082 6      Or(Lshr(Or(Lshr(arg0, arg1), arg0), arg2), ...)
"""

from __future__ import annotations

import argparse
import csv
import sys
from pathlib import Path

from util import expr_arity, expr_to_id


def render_top_inc(pattern_id: str, arity: int) -> str:
    params = ", ".join(
        f"std::array<APInt, 2> ssa_{i}_" for i in range(arity)
    )
    return (
        f"namespace {pattern_id} {{\n"
        f"std::array<APInt, 2> solution({params}) {{\n"
        f"\tunsigned bw = ssa_0_[0].getBitWidth();\n"
        f"\treturn std::array<APInt, 2>{{APInt(bw, 0), APInt(bw, 0)}};\n"
        f"}}\n"
        f"}}\n"
    )


def patterns_from_tsv(
    tsv_path: Path, pattern_col: str = "pattern",
) -> tuple[list[tuple[str, str]], list[tuple[str, str]]]:
    """Return (patterns, failures) from a TSV whose `pattern_col` holds the
    expression string. Each id is derived from its expression via expr_to_id;
    duplicate expressions are dropped and genuine id collisions are reported as
    failures rather than silently overwriting one another.
    """
    with tsv_path.open(newline="") as f:
        reader = csv.DictReader(f, delimiter="\t")
        if reader.fieldnames is None or pattern_col not in reader.fieldnames:
            cols = reader.fieldnames or []
            print(
                f"{tsv_path}: no {pattern_col!r} column (header: {cols})",
                file=sys.stderr,
            )
            return [], []
        rows = [row[pattern_col].strip() for row in reader]

    patterns: list[tuple[str, str]] = []
    failures: list[tuple[str, str]] = []
    seen_ids: dict[str, str] = {}
    for i, expression in enumerate(rows):
        if not expression:
            failures.append((f"row{i}", "empty pattern cell"))
            continue
        try:
            expr_arity(expression)  # validate it has argN tokens
            pid = expr_to_id(expression)
            if pid in seen_ids:
                if seen_ids[pid] != expression:
                    raise ValueError(f"id {pid!r} collides with {seen_ids[pid]!r}")
                continue  # exact duplicate expression: keep just one
            seen_ids[pid] = expression
        except Exception as exc:
            failures.append((f"row{i}", f"{type(exc).__name__}: {exc}"))
            continue
        patterns.append((pid, expression))
    return patterns, failures


def main() -> int:
    p = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    p.add_argument("--tsv", type=Path, required=True,
                   help="TSV file with a `pattern` column of expression strings")
    p.add_argument("--pattern-col", default="pattern",
                   help="Column name holding the expression (default: %(default)s)")
    p.add_argument("--out-dir", type=Path, required=True,
                   help="Output transformer folder: the expression-named "
                        "<id>.inc files are written under inc/, the layout "
                        "generate_pattern_dispatcher.py consumes directly.")
    args = p.parse_args()

    patterns, failures = patterns_from_tsv(args.tsv, args.pattern_col)

    # Emit a transformer folder of expression-named <out-dir>/inc/<id>.inc files.
    # The id is the only place the expression lives: the dispatcher generator
    # decodes it straight from the filename, so there is no patterns.json index.
    inc_dir = args.out_dir / "inc"
    inc_dir.mkdir(parents=True, exist_ok=True)

    for pid, expression in patterns:
        arity = expr_arity(expression)
        (inc_dir / f"{pid}.inc").write_text(render_top_inc(pid, arity))

    print(f"wrote {len(patterns)} inc/<id>.inc files")
    if failures:
        print(f"{len(failures)} failures:", file=sys.stderr)
        for pid, msg in failures:
            print(f"  {pid}: {msg}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
