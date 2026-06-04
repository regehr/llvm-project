#!/usr/bin/env python3
"""Generate top-returning Pattern{ID}::solution .inc files for a set of
patterns, plus a patterns.json index mapping pattern id -> expression string.

Two input sources are supported:

  --mlir-dir DIR   A directory of NNN.mlir pattern files. Each file is parsed
                   with synth_xfer's load_pattern and the file stem (e.g. "004")
                   becomes the pattern id. Requires synth-xfer to be importable,
                   so run with synth-xfer's venv python, e.g.:

                     /home/ubuntu/repos/synth-xfer/venv/bin/python \
                         generate_top_patterns_inc.py \
                         --mlir-dir /home/ubuntu/repos/synth-xfer/mlir/Patterns \
                         --out-dir  .../Generated/patterns_top

  --tsv FILE       A TSV with a header row and a `pattern` column holding the
                   expression string directly (the same format as
                   load_pattern(...).expression), e.g. results/top_1000_pattern.tsv:

                     count   size   pattern
                     6359082 6      Or(Lshr(Or(Lshr(arg0, arg1), arg0), arg2), ...)

                   Pattern ids are assigned sequentially in row order
                   (zero-padded). This mode needs no synth-xfer/xdsl, so any
                   python works.

Exactly one of --mlir-dir / --tsv must be given.
"""

from __future__ import annotations

import argparse
import csv
import json
import re
import sys
from pathlib import Path

ARG_RE = re.compile(r"\barg(\d+)\b")


def expr_arity(expression: str) -> int:
    indices = [int(m.group(1)) for m in ARG_RE.finditer(expression)]
    if not indices:
        raise ValueError(f"No argN tokens in expression: {expression!r}")
    return max(indices) + 1


def render_top_inc(pattern_id: str, arity: int) -> str:
    params = ", ".join(
        f"std::array<APInt, 2> ssa_{i}_" for i in range(arity)
    )
    return (
        f"namespace Pattern{pattern_id} {{\n"
        f"std::array<APInt, 2> solution({params}) {{\n"
        f"\tunsigned bw = ssa_0_[0].getBitWidth();\n"
        f"\treturn std::array<APInt, 2>{{APInt(bw, 0), APInt(bw, 0)}};\n"
        f"}}\n"
        f"}}\n"
    )


def patterns_from_mlir(mlir_dir: Path) -> tuple[list[tuple[str, str]], list[tuple[str, str]]]:
    """Return (patterns, failures) from a directory of NNN.mlir files.

    patterns is a list of (pid, expression); failures is (pid, message).
    """
    from synth_xfer._util.pattern import load_pattern  # noqa: PLC0415

    mlir_files = sorted(mlir_dir.glob("*.mlir"))
    if not mlir_files:
        print(f"no .mlir files found under {mlir_dir}", file=sys.stderr)
        return [], []

    patterns: list[tuple[str, str]] = []
    failures: list[tuple[str, str]] = []
    for mlir_file in mlir_files:
        pid = mlir_file.stem  # e.g. "004"
        try:
            dag = load_pattern(mlir_file)
            expression = dag.expression
            expr_arity(expression)  # validate it has argN tokens
        except Exception as exc:
            failures.append((pid, f"{type(exc).__name__}: {exc}"))
            continue
        patterns.append((pid, expression))
    return patterns, failures


def patterns_from_tsv(
    tsv_path: Path, pattern_col: str = "pattern",
) -> tuple[list[tuple[str, str]], list[tuple[str, str]]]:
    """Return (patterns, failures) from a TSV whose `pattern_col` holds the
    expression string. Pattern ids are assigned sequentially in row order,
    zero-padded to a consistent width.
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

    width = max(3, len(str(len(rows) - 1))) if rows else 3
    patterns: list[tuple[str, str]] = []
    failures: list[tuple[str, str]] = []
    for i, expression in enumerate(rows):
        pid = f"{i:0{width}d}"
        if not expression:
            failures.append((pid, "empty pattern cell"))
            continue
        try:
            expr_arity(expression)  # validate it has argN tokens
        except Exception as exc:
            failures.append((pid, f"{type(exc).__name__}: {exc}"))
            continue
        patterns.append((pid, expression))
    return patterns, failures


def main() -> int:
    p = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    src = p.add_mutually_exclusive_group(required=True)
    src.add_argument("--mlir-dir", type=Path,
                     help="Directory holding NNN.mlir pattern files")
    src.add_argument("--tsv", type=Path,
                     help="TSV file with a `pattern` column of expression strings")
    p.add_argument("--pattern-col", default="pattern",
                   help="Column name holding the expression in --tsv mode "
                        "(default: %(default)s)")
    p.add_argument("--out-dir", type=Path, required=True,
                   help="Output transformer folder: patterns.json is written at "
                        "its root and the pattern_NNN.inc files under inc/, the "
                        "layout generate_pattern_dispatcher.py consumes directly.")
    args = p.parse_args()

    if args.mlir_dir is not None:
        patterns, failures = patterns_from_mlir(args.mlir_dir)
    else:
        patterns, failures = patterns_from_tsv(args.tsv, args.pattern_col)

    # Emit a well-formed transformer folder: <out-dir>/patterns.json plus
    # <out-dir>/inc/pattern_NNN.inc, so the result feeds the dispatcher generator
    # with no further shuffling.
    inc_dir = args.out_dir / "inc"
    inc_dir.mkdir(parents=True, exist_ok=True)

    index: dict[str, str] = {}
    for pid, expression in patterns:
        arity = expr_arity(expression)
        inc_path = inc_dir / f"pattern_{pid}.inc"
        inc_path.write_text(render_top_inc(pid, arity))
        index[pid] = expression

    json_path = args.out_dir / "patterns.json"
    json_path.write_text(
        json.dumps({k: index[k] for k in sorted(index)}, indent=2) + "\n"
    )

    print(f"wrote {len(index)} inc/pattern_*.inc files and {json_path}")
    if failures:
        print(f"{len(failures)} failures:", file=sys.stderr)
        for pid, msg in failures:
            print(f"  {pid}: {msg}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
