#!/usr/bin/env python3

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass
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
OPT_NAME_FLAG = "-non-global-value-max-name-size=-1"
STAT_RE = re.compile(
    r"^\s*(\d+)\s+instcount - Number of (SExt|ZExt|Trunc) insts$",
    re.MULTILINE,
)
PASS_FUNCTION_RE = re.compile(r'Running pass ".*?" on function "([^"]+)"')


@dataclass
class FileCounts:
    ir_file: Path
    before: Counter[str]
    after: Counter[str]


@dataclass
class CrashRecord:
    ir_file: Path
    phase: str
    functions: tuple[str, ...]


class OptFailure(RuntimeError):
    def __init__(self, phase: str, ir_path: Path, opt_bin: Path, proc: subprocess.CompletedProcess[str]):
        self.phase = phase
        self.ir_path = ir_path
        self.opt_bin = opt_bin
        self.stdout = proc.stdout
        self.stderr = proc.stderr

        lines = [f"{phase} failed for {ir_path} with {opt_bin}"]
        if proc.stdout:
            lines.append("stdout:")
            lines.append(proc.stdout.rstrip())
        if proc.stderr:
            lines.append("stderr:")
            lines.append(proc.stderr.rstrip())
        super().__init__("\n".join(lines))


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
        [str(opt_bin), OPT_NAME_FLAG, "--print-passes"],
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
            OPT_NAME_FLAG,
            "-stats",
            "-passes=instcount",
            "-disable-output",
            str(ir_path),
        ],
        text=True,
        capture_output=True,
    )

    if proc.returncode != 0:
        raise OptFailure("instcount", ir_path, opt_bin, proc)

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
            OPT_NAME_FLAG,
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

        raise OptFailure("width-opt", ir_path, opt_bin, proc)

    return temp_path


def extract_crashing_functions(stderr: str) -> tuple[str, ...]:
    functions: list[str] = []
    seen: set[str] = set()
    for match in PASS_FUNCTION_RE.finditer(stderr):
        function_name = match.group(1)
        if function_name in seen:
            continue
        seen.add(function_name)
        functions.append(function_name)
    return tuple(functions)


def process_ir_file(opt_bin: Path, ir_file: Path) -> FileCounts | CrashRecord:
    try:
        before_counts = run_instcount(opt_bin, ir_file)
        temp_ir = run_width_opt_to_temp(opt_bin, ir_file)
        try:
            after_counts = run_instcount(opt_bin, temp_ir)
        finally:
            temp_ir.unlink(missing_ok=True)
    except OptFailure as err:
        return CrashRecord(
            ir_file=ir_file,
            phase=err.phase,
            functions=extract_crashing_functions(err.stderr),
        )

    return FileCounts(ir_file=ir_file, before=before_counts, after=after_counts)


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
    if round(change, 1) == 0.0:
        return "0.0%"
    return f"{change:+.1f}%"


def format_metric_cell(before: int, after: int) -> str:
    return f"{before} -> {after} ({format_percent_change(before, after)})"


def format_seconds(seconds: float) -> str:
    return f"{seconds:.3g}"


def print_table_header() -> None:
    print("| Benchmark | Files | SExt | ZExt | Trunc | Total | Time (s) |")
    print("| --- | ---: | ---: | ---: | ---: | ---: | ---: |")


def print_table_row(
    name: str,
    num_files: int,
    before: Counter[str],
    after: Counter[str],
    wall_clock_seconds: float,
) -> None:
    before_total = sum(before[opcode] for opcode in WIDTH_OPCODES)
    after_total = sum(after[opcode] for opcode in WIDTH_OPCODES)
    print(
        f"| {name} | {num_files} | "
        f"{format_metric_cell(before['sext'], after['sext'])} | "
        f"{format_metric_cell(before['zext'], after['zext'])} | "
        f"{format_metric_cell(before['trunc'], after['trunc'])} | "
        f"{format_metric_cell(before_total, after_total)} | "
        f"{format_seconds(wall_clock_seconds)} |"
    )


def print_crash_report(crashes: list[CrashRecord]) -> None:
    print()
    print("## opt Crashes")
    if not crashes:
        print()
        print("No opt crashes observed.")
        return

    seen: set[tuple[str, str]] = set()
    for crash in crashes:
        if crash.functions:
            for function_name in crash.functions:
                key = (function_name, str(crash.ir_file))
                if key in seen:
                    continue
                seen.add(key)
                print(f"- `{function_name}` in `{crash.ir_file}`")
        else:
            key = ("<unknown>", str(crash.ir_file))
            if key in seen:
                continue
            seen.add(key)
            print(f"- `<unknown>` in `{crash.ir_file}`")


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
    total_wall_clock_seconds = 0.0
    missing_apps: list[str] = []
    apps_with_no_ir: list[str] = []
    crashes: list[CrashRecord] = []

    print_table_header()

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
        app_successful_files = 0
        app_start = time.perf_counter()
        max_workers = min(jobs, len(ir_files))
        with ThreadPoolExecutor(max_workers=max_workers) as executor:
            for result in executor.map(lambda path: process_ir_file(opt_bin, path), ir_files):
                if isinstance(result, CrashRecord):
                    crashes.append(result)
                    continue

                app_counts.update(result.before)
                app_width_opt_counts.update(result.after)
                app_successful_files += 1
        app_wall_clock_seconds = time.perf_counter() - app_start

        print_table_row(
            app_dir.name,
            app_successful_files,
            app_counts,
            app_width_opt_counts,
            app_wall_clock_seconds,
        )
        total_counts.update(app_counts)
        total_width_opt_counts.update(app_width_opt_counts)
        total_files += app_successful_files
        total_wall_clock_seconds += app_wall_clock_seconds

    if missing_apps:
        for app_name in missing_apps:
            print(f"warning: application not found: {app_name}", file=sys.stderr)

    if apps_with_no_ir:
        for app_name in apps_with_no_ir:
            print(f"warning: no optimized .ll files found for: {app_name}", file=sys.stderr)

    print_table_row(
        "TOTAL",
        total_files,
        total_counts,
        total_width_opt_counts,
        total_wall_clock_seconds,
    )

    print_crash_report(crashes)

    return 1 if missing_apps else 0


if __name__ == "__main__":
    raise SystemExit(main())
