#!/usr/bin/env python3

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
from typing import Iterable


WIDTH_OPCODES = ("sext", "zext", "trunc")
STAT_LABELS = {
    "sext": "SExt",
    "zext": "ZExt",
    "trunc": "Trunc",
}
BENCH_ROOT = Path.home() / "llvm-opt-benchmark" / "bench"
FALLBACK_OPT = Path("/home/regehr/tmp/llvm-project-regehr/build/bin/opt")
STAT_RE = re.compile(
    r"^\s*(\d+)\s+instcount - Number of (SExt|ZExt|Trunc) insts$",
    re.MULTILINE,
)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Count LLVM sext/zext/trunc instructions in optimized IR for "
            "applications under a benchmark tree using opt's instcount pass."
        )
    )
    parser.add_argument(
        "applications",
        metavar="APP",
        nargs="*",
        help="Application names under the benchmark root. Defaults to all applications.",
    )
    parser.add_argument(
        "--bench-root",
        type=Path,
        default=BENCH_ROOT,
        help=f"Benchmark root containing one directory per application (default: {BENCH_ROOT})",
    )
    parser.add_argument(
        "--opt-bin",
        type=Path,
        default=None,
        help="Path to the opt executable. Defaults to $LLVM_OPT, then PATH, then the local build/bin/opt.",
    )
    parser.add_argument(
        "--jobs",
        type=int,
        default=os.cpu_count() or 1,
        help="Number of files to process in parallel within each application (default: CPU count).",
    )
    return parser.parse_args()


def find_opt(opt_bin_arg: Path | None) -> Path:
    if opt_bin_arg is not None:
        return opt_bin_arg.expanduser().resolve()

    candidates: list[Path] = []

    # Prefer the repository-local opt because width-opt is specific to this tree.
    candidates.append(FALLBACK_OPT)

    env_opt = os.environ.get("LLVM_OPT")
    if env_opt:
        candidates.append(Path(env_opt).expanduser().resolve())

    path_opt = shutil.which("opt")
    if path_opt:
        candidates.append(Path(path_opt).resolve())

    seen: set[Path] = set()
    for candidate in candidates:
        if candidate in seen or not candidate.is_file():
            continue
        seen.add(candidate)
        if supports_required_passes(candidate):
            return candidate

    return FALLBACK_OPT


def supports_required_passes(opt_bin: Path) -> bool:
    proc = subprocess.run(
        [str(opt_bin), "--print-passes"],
        text=True,
        capture_output=True,
    )
    if proc.returncode != 0:
        return False

    passes = proc.stdout.splitlines()
    return "  instcount" in passes and "  width-opt" in passes


def run_instcount(opt_bin: Path, ir_path: Path) -> Counter[str]:
    proc = subprocess.run(
        [
            str(opt_bin),
            "-stats",
            "-passes=instcount",
            "-disable-output",
            str(ir_path),
        ],
        text=True,
        capture_output=True,
    )

    if proc.returncode != 0:
        lines = [f"instcount failed for {ir_path} with {opt_bin}"]
        if proc.stdout:
            lines.append("stdout:")
            lines.append(proc.stdout.rstrip())
        if proc.stderr:
            lines.append("stderr:")
            lines.append(proc.stderr.rstrip())
        raise RuntimeError("\n".join(lines))

    counts: Counter[str] = Counter()
    for match in STAT_RE.finditer(proc.stderr):
        count = int(match.group(1))
        label = match.group(2)
        for opcode, stat_label in STAT_LABELS.items():
            if label == stat_label:
                counts[opcode] = count
                break

    return counts


def run_width_opt_to_temp(opt_bin: Path, ir_path: Path) -> Path:
    with tempfile.NamedTemporaryFile(
        prefix="width-opt-",
        suffix=".ll",
        delete=False,
    ) as tmp:
        temp_path = Path(tmp.name)

    proc = subprocess.run(
        [
            str(opt_bin),
            "-passes=width-opt",
            "-S",
            str(ir_path),
            "-o",
            str(temp_path),
        ],
        text=True,
        capture_output=True,
    )

    if proc.returncode != 0:
        try:
            temp_path.unlink(missing_ok=True)
        except OSError:
            pass

        lines = [f"width-opt failed for {ir_path} with {opt_bin}"]
        if proc.stdout:
            lines.append("stdout:")
            lines.append(proc.stdout.rstrip())
        if proc.stderr:
            lines.append("stderr:")
            lines.append(proc.stderr.rstrip())
        raise RuntimeError("\n".join(lines))

    return temp_path


def process_ir_file(opt_bin: Path, ir_file: Path) -> tuple[Counter[str], Counter[str]]:
    before_counts = run_instcount(opt_bin, ir_file)
    temp_ir = run_width_opt_to_temp(opt_bin, ir_file)
    try:
        after_counts = run_instcount(opt_bin, temp_ir)
    finally:
        temp_ir.unlink(missing_ok=True)

    return before_counts, after_counts


def find_optimized_ir_files(app_dir: Path) -> list[Path]:
    optimized_dir = app_dir / "optimized"
    if not optimized_dir.is_dir():
        return []
    return sorted(path for path in optimized_dir.rglob("*.ll") if path.is_file())


def iter_app_dirs(bench_root: Path, app_names: list[str]) -> Iterable[Path]:
    if app_names:
        for name in app_names:
            yield bench_root / name
        return

    for path in sorted(bench_root.iterdir()):
        if path.is_dir():
            yield path


def format_percent_change(before: int, after: int) -> str:
    if before == 0:
        if after == 0:
            return "0.0%"
        return "n/a"

    change = ((after - before) / before) * 100.0
    return f"{change:+.1f}%"


def format_metric_cell(before: int, after: int) -> str:
    return f"{before} -> {after} ({format_percent_change(before, after)})"


def print_table_header() -> None:
    print("| Benchmark | Files | SExt | ZExt | Trunc | Total |")
    print("| --- | ---: | ---: | ---: | ---: | ---: |")


def print_table_row(name: str, num_files: int, before: Counter[str], after: Counter[str]) -> None:
    before_total = sum(before[opcode] for opcode in WIDTH_OPCODES)
    after_total = sum(after[opcode] for opcode in WIDTH_OPCODES)
    print(
        f"| {name} | {num_files} | "
        f"{format_metric_cell(before['sext'], after['sext'])} | "
        f"{format_metric_cell(before['zext'], after['zext'])} | "
        f"{format_metric_cell(before['trunc'], after['trunc'])} | "
        f"{format_metric_cell(before_total, after_total)} |"
    )


def main() -> int:
    args = parse_args()
    bench_root = args.bench_root.expanduser().resolve()
    opt_bin = find_opt(args.opt_bin)
    jobs = max(1, args.jobs)

    if not bench_root.is_dir():
        print(f"error: benchmark root does not exist: {bench_root}", file=sys.stderr)
        return 1
    if not opt_bin.is_file():
        print(f"error: opt executable does not exist: {opt_bin}", file=sys.stderr)
        return 1
    if not supports_required_passes(opt_bin):
        print(
            f"error: opt executable does not support both instcount and width-opt: {opt_bin}",
            file=sys.stderr,
        )
        return 1

    total_counts: Counter[str] = Counter()
    total_width_opt_counts: Counter[str] = Counter()
    total_files = 0
    missing_apps: list[str] = []
    apps_with_no_ir: list[str] = []

    print_table_header()

    try:
        for app_dir in iter_app_dirs(bench_root, args.applications):
            if not app_dir.is_dir():
                missing_apps.append(app_dir.name)
                continue

            ir_files = find_optimized_ir_files(app_dir)
            if not ir_files:
                apps_with_no_ir.append(app_dir.name)
                continue

            app_counts: Counter[str] = Counter()
            app_width_opt_counts: Counter[str] = Counter()
            max_workers = min(jobs, len(ir_files))
            with ThreadPoolExecutor(max_workers=max_workers) as executor:
                for before_counts, after_counts in executor.map(
                    lambda path: process_ir_file(opt_bin, path),
                    ir_files,
                ):
                    app_counts.update(before_counts)
                    app_width_opt_counts.update(after_counts)

            print_table_row(app_dir.name, len(ir_files), app_counts, app_width_opt_counts)
            total_counts.update(app_counts)
            total_width_opt_counts.update(app_width_opt_counts)
            total_files += len(ir_files)
    except RuntimeError as err:
        print(f"error: {err}", file=sys.stderr)
        return 1

    if missing_apps:
        for app_name in missing_apps:
            print(f"warning: application not found: {app_name}", file=sys.stderr)

    if apps_with_no_ir:
        for app_name in apps_with_no_ir:
            print(f"warning: no optimized .ll files found for: {app_name}", file=sys.stderr)

    if total_files > 0:
        print_table_row("TOTAL", total_files, total_counts, total_width_opt_counts)

    return 1 if missing_apps else 0


if __name__ == "__main__":
    raise SystemExit(main())
