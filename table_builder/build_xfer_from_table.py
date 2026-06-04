#!/usr/bin/env python3
"""Generate per-pattern KnownBits lookup-table .inc files from TSVs.

This is a faithful transcoder: every data row in the input TSV becomes a row in
the emitted lookup table. It does NOT prune or otherwise change the table
content -- run prune_table.py first if you want top/bottom rows
removed. The only rows skipped here are bottom rows, which cannot be encoded as
ternary masks.

Emission strategy per pattern:
  - Partition rows by bitwidth into separate subtables.
  - For tables with <= --inline-threshold rows AND max bw <= 64, emit an inline
    `constexpr Entry[]` plus a hand-rolled loop in solution().
  - Otherwise emit a byte-string blob and call the shared
    KnownBitsPatterns::lookupKB<> template from table_helper.inc.
  - Patterns with no usable rows (no `ideal` column, or all rows bottom) get a
    stub that returns top.

Patterns without a matching TSV are skipped; their existing .inc is left alone.
"""
from argparse import ArgumentParser
from collections import defaultdict
from pathlib import Path
import re
import sys


def parse_tsv(path: Path):
    """Returns (arity, rows) where rows is a list of (bw, args, ideal).

    `args` is a list of MSB-first '0'/'1'/'?' ternary strings (one per arg) and
    `ideal` is the same. Bottom rows are skipped.
    """
    lines = path.read_text().splitlines()

    arity = None
    in_header = False
    i = 0
    while i < len(lines):
        line = lines[i]
        if line.startswith("# ---"):
            in_header = not in_header
            i += 1
            if not in_header:
                break
            continue
        if in_header:
            m = re.match(r"# arity:\s*(\d+)", line)
            if m:
                arity = int(m.group(1))
        i += 1

    if arity is None:
        raise ValueError(f"{path}: missing arity in YAML header")

    while i < len(lines) and (not lines[i].strip() or lines[i].startswith("#")):
        i += 1
    if i >= len(lines):
        return arity, []
    header = lines[i].split("\t")
    i += 1

    bw_col = header.index("bw")
    arg_cols = [header.index(f"arg_{a}") for a in range(arity)]
    if "ideal" not in header:
        return arity, []
    ideal_col = header.index("ideal")

    rows = []
    for line in lines[i:]:
        if not line.strip() or line.startswith("#"):
            continue
        parts = line.split("\t")
        bw = int(parts[bw_col])
        args = [parts[c] for c in arg_cols]
        ideal = parts[ideal_col]
        if any(s == "(bottom)" for s in args + [ideal]):
            continue
        for s in args + [ideal]:
            if len(s) != bw:
                raise ValueError(
                    f"{path}: ternary string {s!r} does not match bw={bw}"
                )
            if not re.fullmatch(r"[01?]+", s):
                raise ValueError(f"{path}: invalid ternary string {s!r}")
        rows.append((bw, args, ideal))

    return arity, rows


def ternary_to_zo(s: str) -> tuple[int, int]:
    """Convert ternary string (MSB-first) to (zero_mask, one_mask)."""
    bw = len(s)
    zero = 0
    one = 0
    for i, ch in enumerate(s):
        bit = bw - 1 - i
        if ch == "0":
            zero |= 1 << bit
        elif ch == "1":
            one |= 1 << bit
    return zero, one


def int_to_le_bytes(v: int, n: int) -> bytes:
    return v.to_bytes(n, "little")


def emit_stub(pid: str, arity: int) -> str:
    sig = ", ".join(f"std::array<APInt, 2> ssa_{i}_" for i in range(arity))
    return (
        f"namespace {pid} {{\n"
        f"std::array<APInt, 2> solution({sig}) {{\n"
        f"\tunsigned bw = ssa_0_[0].getBitWidth();\n"
        f"\treturn std::array<APInt, 2>{{APInt(bw, 0), APInt(bw, 0)}};\n"
        f"}}\n"
        f"}}\n"
    )


def emit_inline(pid: str, arity: int,
                bw_groups: dict[int, list[tuple[list[str], str]]]) -> str:
    sig = ", ".join(f"std::array<APInt, 2> ssa_{i}_" for i in range(arity))
    out: list[str] = []
    out.append(f"namespace {pid} {{\n")
    out.append("namespace {\n")
    out.append("struct Entry {\n")
    out.append("  unsigned bw;\n")
    out.append(f"  uint64_t argZ[{arity}];\n")
    out.append(f"  uint64_t argO[{arity}];\n")
    out.append("  uint64_t outZ;\n")
    out.append("  uint64_t outO;\n")
    out.append("};\n\n")
    out.append("static constexpr Entry kEntries[] = {\n")
    for bw in sorted(bw_groups):
        for args_str, ideal_str in bw_groups[bw]:
            zos = [ternary_to_zo(s) for s in args_str]
            argZ = ", ".join(f"0x{z:X}ULL" for z, _ in zos)
            argO = ", ".join(f"0x{o:X}ULL" for _, o in zos)
            outZ, outO = ternary_to_zo(ideal_str)
            out.append(
                f"  {{{bw}u, {{{argZ}}}, {{{argO}}}, 0x{outZ:X}ULL, 0x{outO:X}ULL}},\n"
            )
    out.append("};\n")
    out.append("} // namespace\n\n")

    out.append(f"std::array<APInt, 2> solution({sig}) {{\n")
    out.append("  unsigned bw = ssa_0_[0].getBitWidth();\n")
    out.append(
        "  if (bw > 64) return std::array<APInt, 2>{APInt(bw, 0), APInt(bw, 0)};\n"
    )
    out.append(f"  uint64_t inZ[{arity}], inO[{arity}];\n")
    for i in range(arity):
        out.append(f"  inZ[{i}] = ssa_{i}_[0].getZExtValue();\n")
        out.append(f"  inO[{i}] = ssa_{i}_[1].getZExtValue();\n")
    out.append("  uint64_t outZ = 0, outO = 0;\n")
    out.append("  for (const Entry &E : kEntries) {\n")
    out.append("    if (E.bw != bw) continue;\n")
    out.append("    bool match = true;\n")
    out.append(f"    for (unsigned a = 0; a < {arity}; ++a) {{\n")
    out.append(
        "      if ((E.argZ[a] & ~inZ[a]) | (E.argO[a] & ~inO[a])) { match = false; break; }\n"
    )
    out.append("    }\n")
    out.append("    if (!match) continue;\n")
    out.append("    outZ |= E.outZ;\n")
    out.append("    outO |= E.outO;\n")
    out.append("  }\n")
    out.append(
        "  return std::array<APInt, 2>{APInt(bw, outZ), APInt(bw, outO)};\n"
    )
    out.append("}\n")
    out.append("}\n")
    return "".join(out)


# Adjacent string literals are concatenated into a single object, which the
# standard only guarantees can hold 65536 bytes. Keep each blob array (= one
# concatenated literal) safely under that by chunking rows across arrays.
BLOB_LIMIT = 60000


def emit_blob(pid: str, arity: int,
              bw_groups: dict[int, list[tuple[list[str], str]]]) -> str:
    sig = ", ".join(f"std::array<APInt, 2> ssa_{i}_" for i in range(arity))
    out: list[str] = []
    out.append(f"namespace {pid} {{\n")
    out.append("namespace {\n")

    table_entries: list[tuple[int, str, int]] = []
    for bw in sorted(bw_groups):
        mask_bytes = (bw + 7) // 8
        row_bytes = 2 * (arity + 1) * mask_bytes
        rows = bw_groups[bw]
        rows_per_chunk = max(1, BLOB_LIMIT // row_bytes)

        for chunk_idx, start in enumerate(range(0, len(rows), rows_per_chunk)):
            chunk = rows[start:start + rows_per_chunk]
            blob_name = f"kBlob_bw{bw}_{chunk_idx}"
            out.append(f"static const unsigned char {blob_name}[] =\n")
            for r_idx, (args_str, ideal_str) in enumerate(chunk):
                row = bytearray()
                for s in args_str:
                    z, o = ternary_to_zo(s)
                    row.extend(int_to_le_bytes(z, mask_bytes))
                    row.extend(int_to_le_bytes(o, mask_bytes))
                outZ, outO = ternary_to_zo(ideal_str)
                row.extend(int_to_le_bytes(outZ, mask_bytes))
                row.extend(int_to_le_bytes(outO, mask_bytes))
                assert len(row) == row_bytes
                esc = "".join(f"\\x{b:02x}" for b in row)
                terminator = ";" if r_idx == len(chunk) - 1 else ""
                out.append(f'    "{esc}"{terminator}\n')
            table_entries.append((bw, blob_name, len(chunk)))

    out.append("\n")
    out.append("static const ::KnownBitsPatterns::BwTable kTables[] = {\n")
    for bw, blob_name, n_rows in table_entries:
        out.append(f"  {{{bw}u, {n_rows}u, {blob_name}}},\n")
    out.append("};\n")
    out.append("} // namespace\n\n")

    out.append(f"std::array<APInt, 2> solution({sig}) {{\n")
    arg_list = ", ".join(f"ssa_{i}_" for i in range(arity))
    out.append(
        f"  const std::array<APInt, 2> args[{arity}] = {{{arg_list}}};\n"
    )
    out.append(
        f"  return ::KnownBitsPatterns::lookupKB<{arity}>(args, kTables, std::size(kTables));\n"
    )
    out.append("}\n")
    out.append("}\n")
    return "".join(out)


def main() -> None:
    ap = ArgumentParser()
    ap.add_argument(
        "--tsv-dir", type=Path, default=Path("results/tsv/ideal_optimized"),
        help="Directory containing <id>.tsv files",
    )
    ap.add_argument(
        "--out-dir", type=Path,
        default=Path("llvm/lib/Analysis/Generated/patterns"),
        help="Directory to write <id>.inc files into",
    )
    ap.add_argument(
        "--inline-threshold", type=int, default=16,
        help=(
            "Patterns whose total row count is <= this AND whose max bw <= 64 "
            "are emitted as an inline constexpr Entry[]; larger patterns use "
            "the byte-blob format."
        ),
    )
    args = ap.parse_args()

    tsv_files = sorted(args.tsv_dir.glob("*.tsv"))
    if not tsv_files:
        print(f"No TSVs found in {args.tsv_dir}", file=sys.stderr)
        sys.exit(1)

    args.out_dir.mkdir(parents=True, exist_ok=True)

    n_stub = 0
    n_inline = 0
    n_blob = 0
    n_rows = 0

    for tsv_path in tsv_files:
        # The filename stem is the pattern id (the expression-derived name).
        pid = tsv_path.stem
        out_path = args.out_dir / f"{pid}.inc"

        arity, rows = parse_tsv(tsv_path)
        n_rows += len(rows)

        bw_groups: dict[int, list[tuple[list[str], str]]] = defaultdict(list)
        for bw, args_str, ideal_str in rows:
            bw_groups[bw].append((args_str, ideal_str))

        if not bw_groups:
            content = emit_stub(pid, arity)
            n_stub += 1
        else:
            total_rows = sum(len(v) for v in bw_groups.values())
            max_bw = max(bw_groups)
            if max_bw <= 64 and total_rows <= args.inline_threshold:
                content = emit_inline(pid, arity, bw_groups)
                n_inline += 1
            else:
                content = emit_blob(pid, arity, bw_groups)
                n_blob += 1

        out_path.write_text(content)

    print(f"Processed {len(tsv_files)} TSVs from {args.tsv_dir}:")
    print(f"  stub (no usable rows): {n_stub}")
    print(f"  inline Entry[]:        {n_inline}")
    print(f"  byte blob:             {n_blob}")
    print(f"Rows emitted: {n_rows}")


if __name__ == "__main__":
    main()
