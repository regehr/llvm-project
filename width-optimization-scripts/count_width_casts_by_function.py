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
FALLBACK_LLVM_EXTRACT = Path("/home/regehr/tmp/llvm-project-regehr/build/bin/llvm-extract")
OPT_NAME_FLAG = "-non-global-value-max-name-size=-1"
STAT_RE = re.compile(
    r"^\s*(\d+)\s+instcount - Number of (SExt|ZExt|Trunc) insts$",
    re.MULTILINE,
)
TOTAL_INSTS_RE = re.compile(
    r"^\s*(\d+)\s+instcount - Number of instructions \(of all types\)$",
    re.MULTILINE,
)
PASS_FUNCTION_RE = re.compile(r'Running pass ".*?" on function "([^"]+)"')
FUNCTION_DEF_RE = re.compile(
    r'^\s*define\b.*?@(?P<name>"(?:[^"\\]|\\.)*"|[^\s(]+)\('
)


@dataclass
class FunctionCounts:
    benchmark: str
    ir_file: Path
    function_name: str
    before: Counter[str]
    after: Counter[str]
    before_total_insts: int
    after_total_insts: int
    width_opt_seconds: float


@dataclass
class TotalRow:
    label: str
    before: Counter[str]
    after: Counter[str]
    before_total_insts: int
    after_total_insts: int
    width_opt_seconds: float


@dataclass
class CrashRecord:
    benchmark: str
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


class ExtractFailure(RuntimeError):
    pass


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Count LLVM sext/zext/trunc instructions per function in optimized "
            "benchmark IR before and after width-opt."
        )
    )
    parser.add_argument(
        "benchmarks",
        metavar="BENCHMARK",
        nargs="+",
        help="Benchmark names under the benchmark root.",
    )
    parser.add_argument(
        "--bench-root",
        type=Path,
        default=BENCH_ROOT,
        help=f"Benchmark root containing one directory per benchmark (default: {BENCH_ROOT})",
    )
    parser.add_argument(
        "--opt-bin",
        type=Path,
        default=None,
        help="Path to the opt executable. Defaults to $LLVM_OPT, then PATH, then the local build/bin/opt.",
    )
    parser.add_argument(
        "--llvm-extract-bin",
        type=Path,
        default=None,
        help="Path to the llvm-extract executable. Defaults to PATH, then the local build/bin/llvm-extract.",
    )
    parser.add_argument(
        "--jobs",
        type=int,
        default=os.cpu_count() or 1,
        help="Number of IR files to process in parallel within each benchmark (default: CPU count).",
    )
    return parser.parse_args()


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


def find_opt(opt_bin_arg: Path | None) -> Path:
    if opt_bin_arg is not None:
        return opt_bin_arg.expanduser().resolve()

    candidates: list[Path] = [FALLBACK_OPT]

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


def find_llvm_extract(llvm_extract_arg: Path | None) -> Path:
    if llvm_extract_arg is not None:
        return llvm_extract_arg.expanduser().resolve()

    path_extract = shutil.which("llvm-extract")
    if path_extract:
        return Path(path_extract).resolve()

    return FALLBACK_LLVM_EXTRACT


def run_instcount(opt_bin: Path, ir_path: Path) -> tuple[Counter[str], int]:
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

    total_match = TOTAL_INSTS_RE.search(proc.stderr)
    total_insts = int(total_match.group(1)) if total_match else 0

    return counts, total_insts


def run_width_opt_to_temp(opt_bin: Path, ir_path: Path) -> tuple[Path, float]:
    with tempfile.NamedTemporaryFile(prefix="width-opt-", suffix=".ll", delete=False) as tmp:
        temp_path = Path(tmp.name)

    start = time.perf_counter()
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
    elapsed = time.perf_counter() - start

    if proc.returncode != 0:
        temp_path.unlink(missing_ok=True)
        raise OptFailure("width-opt", ir_path, opt_bin, proc)

    return temp_path, elapsed


def create_temp_ir_path(prefix: str) -> Path:
    with tempfile.NamedTemporaryFile(prefix=prefix, suffix=".ll", delete=False) as tmp:
        return Path(tmp.name)


def extract_function_to_temp(llvm_extract_bin: Path, ir_path: Path, function_name: str) -> Path:
    temp_path = create_temp_ir_path("llvm-extract-")
    proc = subprocess.run(
        [
            str(llvm_extract_bin),
            f"--func={function_name}",
            str(ir_path),
            "-S",
            "-o",
            str(temp_path),
        ],
        text=True,
        capture_output=True,
    )

    if proc.returncode != 0:
        temp_path.unlink(missing_ok=True)
        lines = [f"llvm-extract failed for function {function_name} in {ir_path} with {llvm_extract_bin}"]
        if proc.stdout:
            lines.append("stdout:")
            lines.append(proc.stdout.rstrip())
        if proc.stderr:
            lines.append("stderr:")
            lines.append(proc.stderr.rstrip())
        raise ExtractFailure("\n".join(lines))

    return temp_path


def decode_ir_name(token: str) -> str:
    if not (token.startswith('"') and token.endswith('"')):
        return token

    body = token[1:-1]

    def repl(match: re.Match[str]) -> str:
        return chr(int(match.group(1), 16))

    return re.sub(r"\\([0-9A-Fa-f]{2})", repl, body)


def parse_function_names(ir_path: Path) -> list[str]:
    names: list[str] = []
    seen: set[str] = set()

    with ir_path.open("r", encoding="utf-8", errors="replace") as handle:
        for line in handle:
            match = FUNCTION_DEF_RE.match(line)
            if not match:
                continue
            name = decode_ir_name(match.group("name"))
            if name in seen:
                continue
            seen.add(name)
            names.append(name)

    return names


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


def process_ir_file(
    benchmark: str,
    ir_file: Path,
    optimized_root: Path,
    opt_bin: Path,
    llvm_extract_bin: Path,
) -> list[FunctionCounts] | CrashRecord:
    function_names = parse_function_names(ir_file)
    if not function_names:
        return []

    try:
        rows: list[FunctionCounts] = []
        relative_ir = ir_file.relative_to(optimized_root)
        for function_name in function_names:
            before_extract = extract_function_to_temp(llvm_extract_bin, ir_file, function_name)
            try:
                before_counts, before_total_insts = run_instcount(opt_bin, before_extract)
                try:
                    after_width_opt_file, width_opt_seconds = run_width_opt_to_temp(opt_bin, before_extract)
                except OptFailure as err:
                    return CrashRecord(
                        benchmark=benchmark,
                        ir_file=relative_ir,
                        phase=err.phase,
                        functions=extract_crashing_functions(err.stderr) or (function_name,),
                    )
                try:
                    after_counts, after_total_insts = run_instcount(opt_bin, after_width_opt_file)
                finally:
                    after_width_opt_file.unlink(missing_ok=True)
            except OptFailure:
                # instcount failures should be treated like a skipped file as well.
                # They do not usually include a useful function marker, so use the
                # function currently being processed.
                return CrashRecord(
                    benchmark=benchmark,
                    ir_file=relative_ir,
                    phase="instcount",
                    functions=(function_name,),
                )
            finally:
                before_extract.unlink(missing_ok=True)

            rows.append(
                FunctionCounts(
                    benchmark=benchmark,
                    ir_file=relative_ir,
                    function_name=function_name,
                    before=before_counts,
                    after=after_counts,
                    before_total_insts=before_total_insts,
                    after_total_insts=after_total_insts,
                    width_opt_seconds=width_opt_seconds,
                )
            )
        return rows
    except ExtractFailure:
        return []


def find_optimized_ir_files(benchmark_dir: Path) -> list[Path]:
    optimized_dir = benchmark_dir / "optimized"
    if not optimized_dir.is_dir():
        return []
    return sorted(path for path in optimized_dir.rglob("*.ll") if path.is_file())


def iter_benchmark_dirs(bench_root: Path, benchmark_names: list[str]) -> Iterable[Path]:
    for name in benchmark_names:
        yield bench_root / name


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


def escape_markdown_cell(text: str) -> str:
    return text.replace("\\", "\\\\").replace("|", "\\|")


def print_table_header() -> None:
    print("| Benchmark | IR File | Function | Insts | SExt | ZExt | Trunc | Total | WidthOpt Time (s) |")
    print("| --- | --- | --- | ---: | ---: | ---: | ---: | ---: | ---: |")


def print_function_row(row: FunctionCounts) -> None:
    before_total = sum(row.before[opcode] for opcode in WIDTH_OPCODES)
    after_total = sum(row.after[opcode] for opcode in WIDTH_OPCODES)
    print(
        f"| {escape_markdown_cell(row.benchmark)} | "
        f"{escape_markdown_cell(str(row.ir_file))} | "
        f"{escape_markdown_cell(row.function_name)} | "
        f"{format_metric_cell(row.before_total_insts, row.after_total_insts)} | "
        f"{format_metric_cell(row.before['sext'], row.after['sext'])} | "
        f"{format_metric_cell(row.before['zext'], row.after['zext'])} | "
        f"{format_metric_cell(row.before['trunc'], row.after['trunc'])} | "
        f"{format_metric_cell(before_total, after_total)} | "
        f"{row.width_opt_seconds:.6f} |"
    )


def print_total_row(
    label: str,
    before: Counter[str],
    after: Counter[str],
    before_total_insts: int,
    after_total_insts: int,
    width_opt_seconds: float,
) -> None:
    before_total = sum(before[opcode] for opcode in WIDTH_OPCODES)
    after_total = sum(after[opcode] for opcode in WIDTH_OPCODES)
    print(
        f"| {escape_markdown_cell(label)} |  |  | "
        f"{format_metric_cell(before_total_insts, after_total_insts)} | "
        f"{format_metric_cell(before['sext'], after['sext'])} | "
        f"{format_metric_cell(before['zext'], after['zext'])} | "
        f"{format_metric_cell(before['trunc'], after['trunc'])} | "
        f"{format_metric_cell(before_total, after_total)} | "
        f"{width_opt_seconds:.6f} |"
    )


def print_crash_report(crashes: list[CrashRecord]) -> None:
    print()
    print("## opt Crashes")
    if not crashes:
        print()
        print("No opt crashes observed.")
        return

    seen: set[tuple[str, str, str]] = set()
    for crash in crashes:
        if crash.functions:
            for function_name in crash.functions:
                key = (crash.benchmark, str(crash.ir_file), function_name)
                if key in seen:
                    continue
                seen.add(key)
                print(f"- `{function_name}` in `{crash.benchmark}/{crash.ir_file}`")
        else:
            key = (crash.benchmark, str(crash.ir_file), "<unknown>")
            if key in seen:
                continue
            seen.add(key)
            print(f"- `<unknown>` in `{crash.benchmark}/{crash.ir_file}`")


def main() -> int:
    args = parse_args()
    bench_root = args.bench_root.expanduser().resolve()
    opt_bin = find_opt(args.opt_bin)
    llvm_extract_bin = find_llvm_extract(args.llvm_extract_bin)
    jobs = max(1, args.jobs)

    if not bench_root.is_dir():
        print(f"error: benchmark root does not exist: {bench_root}", file=sys.stderr)
        return 1
    if not opt_bin.is_file():
        print(f"error: opt executable does not exist: {opt_bin}", file=sys.stderr)
        return 1
    if not llvm_extract_bin.is_file():
        print(f"error: llvm-extract executable does not exist: {llvm_extract_bin}", file=sys.stderr)
        return 1
    if not supports_required_passes(opt_bin):
        print(
            f"error: opt executable does not support both instcount and width-opt: {opt_bin}",
            file=sys.stderr,
        )
        return 1

    total_before: Counter[str] = Counter()
    total_after: Counter[str] = Counter()
    total_before_insts = 0
    total_after_insts = 0
    total_width_opt_seconds = 0.0
    missing_benchmarks: list[str] = []
    benchmarks_with_no_ir: list[str] = []
    crashes: list[CrashRecord] = []
    all_rows: list[FunctionCounts] = []
    benchmark_totals: list[TotalRow] = []

    for benchmark_dir in iter_benchmark_dirs(bench_root, args.benchmarks):
        benchmark_name = benchmark_dir.name
        if not benchmark_dir.is_dir():
            missing_benchmarks.append(benchmark_name)
            continue

        ir_files = find_optimized_ir_files(benchmark_dir)
        if not ir_files:
            benchmarks_with_no_ir.append(benchmark_name)
            continue

        benchmark_before: Counter[str] = Counter()
        benchmark_after: Counter[str] = Counter()
        benchmark_before_insts = 0
        benchmark_after_insts = 0
        benchmark_width_opt_seconds = 0.0
        optimized_root = benchmark_dir / "optimized"
        max_workers = min(jobs, len(ir_files))

        with ThreadPoolExecutor(max_workers=max_workers) as executor:
            for result in executor.map(
                lambda path: process_ir_file(
                    benchmark_name, path, optimized_root, opt_bin, llvm_extract_bin
                ),
                ir_files,
            ):
                if isinstance(result, CrashRecord):
                    crashes.append(result)
                    continue

                for row in result:
                    all_rows.append(row)
                    benchmark_before.update(row.before)
                    benchmark_after.update(row.after)
                    benchmark_before_insts += row.before_total_insts
                    benchmark_after_insts += row.after_total_insts
                    benchmark_width_opt_seconds += row.width_opt_seconds
                    total_before.update(row.before)
                    total_after.update(row.after)
                    total_before_insts += row.before_total_insts
                    total_after_insts += row.after_total_insts
                    total_width_opt_seconds += row.width_opt_seconds

        benchmark_totals.append(
            TotalRow(
                label=f"{benchmark_name} TOTAL",
                before=benchmark_before,
                after=benchmark_after,
                before_total_insts=benchmark_before_insts,
                after_total_insts=benchmark_after_insts,
                width_opt_seconds=benchmark_width_opt_seconds,
            )
        )

    print_table_header()
    for row in sorted(all_rows, key=lambda row: row.width_opt_seconds, reverse=True):
        print_function_row(row)
    for total_row in benchmark_totals:
        print_total_row(
            total_row.label,
            total_row.before,
            total_row.after,
            total_row.before_total_insts,
            total_row.after_total_insts,
            total_row.width_opt_seconds,
        )

    print_total_row(
        "TOTAL",
        total_before,
        total_after,
        total_before_insts,
        total_after_insts,
        total_width_opt_seconds,
    )
    print_crash_report(crashes)

    if missing_benchmarks:
        for benchmark_name in missing_benchmarks:
            print(f"warning: benchmark not found: {benchmark_name}", file=sys.stderr)

    if benchmarks_with_no_ir:
        for benchmark_name in benchmarks_with_no_ir:
            print(f"warning: no optimized .ll files found for: {benchmark_name}", file=sys.stderr)

    return 1 if missing_benchmarks else 0


if __name__ == "__main__":
    raise SystemExit(main())
