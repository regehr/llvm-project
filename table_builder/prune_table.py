#!/usr/bin/env python3
"""Optimize KnownBits pattern TSVs for lookup-table generation.

Reads raw TSVs (one per pattern) and writes optimized copies containing only
the rows worth keeping for a sound + optimal meet-based lookup:

  - Drop rows whose ideal output is top (all '?'); under meet semantics they
    contribute nothing.
  - Drop rows where any arg or the ideal is bottom (unreachable input); those
    cannot be represented as ternary masks and carry no lookup value.

The output preserves the original schema (YAML header block + column header +
kept data rows), so generate_pattern_inc.py can consume it unchanged. TSVs
without an `ideal` column are copied through verbatim (nothing to optimize).
"""
from argparse import ArgumentParser, BooleanOptionalAction
from collections import defaultdict
from pathlib import Path
import re
import sys


def split_tsv(path: Path):
    """Split a TSV into (header_lines, col_header, data_lines, arity).

    header_lines: the YAML comment block, including both '# ---' delimiters.
    col_header:   the tab-separated column header line.
    data_lines:   raw data row strings (stray '#' comment lines removed).
    arity:        parsed from the YAML header, or None if absent.
    """
    lines = path.read_text().splitlines()
    arity = None
    header_lines: list[str] = []
    i = 0

    if i < len(lines) and lines[i].startswith("# ---"):
        header_lines.append(lines[i])
        i += 1
        while i < len(lines) and not lines[i].startswith("# ---"):
            m = re.match(r"# arity:\s*(\d+)", lines[i])
            if m:
                arity = int(m.group(1))
            header_lines.append(lines[i])
            i += 1
        if i < len(lines):  # closing '# ---'
            header_lines.append(lines[i])
            i += 1

    while i < len(lines) and (not lines[i].strip() or lines[i].startswith("#")):
        i += 1
    col_header = lines[i] if i < len(lines) else ""
    i += 1

    data_lines = [l for l in lines[i:] if l.strip() and not l.startswith("#")]
    return header_lines, col_header, data_lines, arity


def is_top(s: str) -> bool:
    return s != "" and all(c == "?" for c in s)


def ternary_masks(s: str) -> tuple[int, int]:
    """Convert a ternary string to (zero_mask, one_mask). Bit position is
    irrelevant for the subset comparisons below, as long as it is consistent
    across rows of the same width -- so we just walk LSB-first."""
    z = o = 0
    for i, ch in enumerate(reversed(s)):
        if ch == "0":
            z |= 1 << i
        elif ch == "1":
            o |= 1 << i
    return z, o


def prune_group(rows: list[tuple[tuple[str, ...], str]]) -> list[bool]:
    """Minimal-cover pruning for rows of one bitwidth under OR/meet semantics.

    A row matches an input iff its known arg bits are a subset of (and agree
    with) the input's known bits; the lookup ORs the outputs of every matching
    row. So row B is redundant if some other row A is strictly more general
    (A's arg constraints are a subset of B's) AND A's output already covers
    B's output bits: whenever B matches, A matches too and contributes at least
    everything B would. Dropping B leaves the transfer function unchanged.

    Returns a keep-flag per input row (original order preserved).
    """
    n = len(rows)
    # Per-row masks: combined arg (zero/one) masks, ideal (zero/one) masks, and
    # the count of known arg bits (a dominator can only have <= as many).
    feats = []
    for args, ideal in rows:
        aZ = aO = shift = 0
        for s in args:
            z, o = ternary_masks(s)
            aZ |= z << shift
            aO |= o << shift
            shift += len(s)
        oZ, oO = ternary_masks(ideal)
        feats.append((aZ, aO, oZ, oO, bin(aZ | aO).count("1")))

    keep = [True] * n
    # Drop exact duplicates first (identical args AND ideal). This is the only
    # way two rows could mutually dominate, so removing the extras up front
    # makes the dominance relation below a strict partial order -- safe to apply
    # in a single pass.
    seen: set = set()
    for i in range(n):
        key = feats[i][:4]
        if key in seen:
            keep[i] = False
        else:
            seen.add(key)

    # Survivors ordered by known-arg-bit count ascending: a dominator A of B has
    # akc <= bkc, so once akc > bkc the rest (also larger) cannot dominate B.
    order = sorted((i for i in range(n) if keep[i]), key=lambda i: feats[i][4])
    for B in order:
        bZ, bO, boZ, boO, bkc = feats[B]
        for A in order:
            if A == B:
                continue
            aZ, aO, _, _, akc = feats[A]
            if akc > bkc:
                break
            # A's arg constraints subset of B's (A more general / agrees)?
            if (aZ & ~bZ) or (aO & ~bO):
                continue
            # A's output covers B's (B's known output bits subset of A's)?
            _, _, aoZ, aoO, _ = feats[A]
            if (boZ & ~aoZ) or (boO & ~aoO):
                continue
            keep[B] = False
            break
    return keep


def main() -> None:
    ap = ArgumentParser()
    ap.add_argument("--tsv-dir", type=Path, default=Path("results/ideal"))
    ap.add_argument("--out-dir", type=Path,
                    default=Path("results/ideal_optimized"))
    ap.add_argument(
        "--drop-top", action=BooleanOptionalAction, default=True,
        help="Drop rows whose ideal output is top (all '?'). Use "
             "--no-drop-top to keep them.",
    )
    ap.add_argument(
        "--prune-subsumed", action=BooleanOptionalAction, default=True,
        help="Drop rows subsumed by a more-general row that already covers "
             "their output (minimal-cover pruning). Use --no-prune-subsumed "
             "to disable.",
    )
    ap.add_argument(
        "--max-rows-per-bw", type=int, default=0, metavar="N",
        help="Cap the number of kept rows per bitwidth: if a bitwidth has more "
             "than N surviving rows, keep only the first N (in original "
             "order). Default 0 (no cap); a common value is 200.",
    )
    args = ap.parse_args()

    tsv_files = sorted(args.tsv_dir.glob("pattern_*.tsv"))
    if not tsv_files:
        print(f"No TSVs found in {args.tsv_dir}", file=sys.stderr)
        sys.exit(1)
    args.out_dir.mkdir(parents=True, exist_ok=True)

    n_seen = 0
    n_kept = 0
    n_top = 0
    n_bottom = 0
    n_subsumed = 0
    n_capped = 0
    for path in tsv_files:
        header_lines, col_header, data_lines, arity = split_tsv(path)
        cols = col_header.split("\t")

        out_lines = list(header_lines)
        out_lines.append(col_header)

        if arity is not None and "ideal" in cols:
            ideal_col = cols.index("ideal")
            arg_cols = [cols.index(f"arg_{a}") for a in range(arity)]
            bw_col = cols.index("bw") if "bw" in cols else None

            # First pass: drop top/bottom rows and bucket the survivors by
            # bitwidth (rows of different widths can never match the same
            # query, so pruning is per-width). Track original line indices so
            # we can emit kept rows in their original order.
            groups: dict[str, list[tuple[int, tuple[str, ...], str]]] = \
                defaultdict(list)
            survive: set[int] = set()
            for idx, line in enumerate(data_lines):
                parts = line.split("\t")
                argvals = tuple(parts[c] for c in arg_cols)
                ideal = parts[ideal_col]
                n_seen += 1
                if any(f == "(bottom)" for f in argvals + (ideal,)):
                    n_bottom += 1
                    continue
                if args.drop_top and is_top(ideal):
                    n_top += 1
                    continue
                survive.add(idx)
                bw = parts[bw_col] if bw_col is not None else str(len(ideal))
                groups[bw].append((idx, argvals, ideal))

            # Second pass: subsumption-prune each width bucket.
            dropped: set[int] = set()
            if args.prune_subsumed:
                for grp in groups.values():
                    flags = prune_group([(a, d) for _, a, d in grp])
                    for (idx, _, _), keep in zip(grp, flags):
                        if not keep:
                            dropped.add(idx)
                            n_subsumed += 1

            cap = args.max_rows_per_bw
            kept_by_bw: dict[str, int] = defaultdict(int)
            for idx, line in enumerate(data_lines):
                if idx in survive and idx not in dropped:
                    bw = line.split("\t")[bw_col] if bw_col is not None \
                        else str(len(line.split("\t")[ideal_col]))
                    if cap and kept_by_bw[bw] >= cap:
                        n_capped += 1
                        continue
                    out_lines.append(line)
                    n_kept += 1
                    kept_by_bw[bw] += 1

            pattern_kept = sum(kept_by_bw.values())
            if kept_by_bw:
                top_bw, top_bw_rows = max(kept_by_bw.items(),
                                          key=lambda kv: kv[1])
            else:
                top_bw, top_bw_rows = "-", 0
            print(f"{path.name:<18}kept: {pattern_kept:>5}   "
                  f"largest: {top_bw_rows:>5} (bw={top_bw})")
        else:
            out_lines.extend(data_lines)

        (args.out_dir / path.name).write_text("\n".join(out_lines) + "\n")

    print(f"Optimized {len(tsv_files)} TSVs -> {args.out_dir}")
    print(f"Rows seen: {n_seen}  kept: {n_kept}  "
          f"dropped: {n_seen - n_kept} "
          f"(top: {n_top}, bottom: {n_bottom}, subsumed: {n_subsumed}, "
          f"capped: {n_capped})")


if __name__ == "__main__":
    main()
