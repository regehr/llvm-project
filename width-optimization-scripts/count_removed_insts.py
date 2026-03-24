#!/usr/bin/env python3

import argparse
import os
import re
import shutil
import subprocess
import sys
from collections import Counter
from pathlib import Path
from typing import Iterable, Optional, Sequence


FALLBACK_OPT = Path("/Users/regehr/llvm-project/for-alive/bin/opt")
WIDTH_OPCODES = ("sext", "zext", "trunc")
OPCODES = {
    "add",
    "addrspacecast",
    "alloca",
    "and",
    "ashr",
    "atomicrmw",
    "bitcast",
    "br",
    "call",
    "callbr",
    "catchpad",
    "catchret",
    "catchswitch",
    "cleanupret",
    "cleanuppad",
    "cmpxchg",
    "extractelement",
    "extractvalue",
    "fadd",
    "fcmp",
    "fdiv",
    "fence",
    "fmul",
    "fneg",
    "fpext",
    "fptosi",
    "fptoui",
    "fptrunc",
    "freeze",
    "frem",
    "fsub",
    "getelementptr",
    "icmp",
    "indirectbr",
    "insertelement",
    "insertvalue",
    "inttoptr",
    "invoke",
    "landingpad",
    "load",
    "lshr",
    "mul",
    "or",
    "phi",
    "ptrtoint",
    "resume",
    "ret",
    "sdiv",
    "select",
    "sext",
    "shl",
    "shufflevector",
    "sitofp",
    "srem",
    "store",
    "sub",
    "switch",
    "trunc",
    "udiv",
    "uitofp",
    "unreachable",
    "urem",
    "va_arg",
    "xor",
    "zext",
}
TOKEN_RE = re.compile(r"[A-Za-z_][A-Za-z0-9._-]*")


def run(cmd: Sequence[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(list(cmd), text=True, capture_output=True)


def candidate_plugins(repo_root: Path) -> Iterable[Path]:
    env_plugin = os.environ.get("WIDTH_OPT_PLUGIN")
    if env_plugin:
        yield Path(env_plugin)

    for suffix in ("so", "dylib", "dll"):
        yield repo_root / "build" / "lib" / f"libWidthOpt.{suffix}"
        yield repo_root.parent / "tmp" / "llvm-width-optimization-build" / "lib" / f"libWidthOpt.{suffix}"
        yield Path("/tmp/llvm-width-optimization-build") / "lib" / f"libWidthOpt.{suffix}"
        yield Path("/tmp/llvm-width-optimization-build-fixpoint") / "lib" / f"libWidthOpt.{suffix}"
        yield Path("/Users/regehr/tmp/llvm-width-optimization-build") / "lib" / f"libWidthOpt.{suffix}"
        yield Path("/Users/regehr/tmp/llvm-width-optimization-build-fixpoint") / "lib" / f"libWidthOpt.{suffix}"


def find_default_plugin(repo_root: Path) -> Optional[Path]:
    for candidate in candidate_plugins(repo_root):
        if candidate.exists():
            return candidate.resolve()
    return None


def find_default_opt() -> Path:
    env_opt = os.environ.get("LLVM_OPT")
    if env_opt:
        return Path(env_opt).resolve()

    which_opt = shutil.which("opt")
    if which_opt:
        return Path(which_opt).resolve()

    return FALLBACK_OPT


def render_input_as_text(opt_bin: Path, src: Path) -> subprocess.CompletedProcess[str]:
    return run(
        [
            str(opt_bin),
            "-non-global-value-max-name-size=-1",
            "-S",
            str(src),
            "-o",
            "-",
        ]
    )


def optimize_to_text(opt_bin: Path, plugin: Path, src: Path) -> subprocess.CompletedProcess[str]:
    return run(
        [
            str(opt_bin),
            "-non-global-value-max-name-size=-1",
            "-load-pass-plugin",
            str(plugin),
            "-passes=width-opt",
            "-S",
            str(src),
            "-o",
            "-",
        ]
    )


def strip_comment(line: str) -> str:
    in_quote = False
    escaped = False

    for index, char in enumerate(line):
        if char == '"' and not escaped:
            in_quote = not in_quote
        elif char == ";" and not in_quote:
            return line[:index]

        escaped = char == "\\" and not escaped
        if char != "\\":
            escaped = False

    return line


def extract_opcode(line: str) -> Optional[str]:
    if "=" in line:
        line = line.split("=", 1)[1].strip()

    for token in TOKEN_RE.findall(line):
        if token in OPCODES:
            return token
    return None


def count_instructions(ir: str) -> Counter[str]:
    counts: Counter[str] = Counter()
    in_function = False

    for raw_line in ir.splitlines():
        line = strip_comment(raw_line).strip()
        if not line:
            continue

        if not in_function:
            if line.startswith("define "):
                in_function = True
            continue

        if line == "}":
            in_function = False
            continue
        if line.endswith(":"):
            continue

        opcode = extract_opcode(line)
        if opcode is not None:
            counts[opcode] += 1

    return counts


def format_failure(kind: str, path: Path, proc: subprocess.CompletedProcess[str]) -> str:
    lines = [f"{kind} failed for {path}"]
    if proc.stdout:
        lines.append("stdout:")
        lines.append(proc.stdout.rstrip())
    if proc.stderr:
        lines.append("stderr:")
        lines.append(proc.stderr.rstrip())
    return "\n".join(lines)


def print_delta_block(title: str, counts: Counter[str]) -> None:
    if not counts:
        print(f"  {title}: none")
        return

    total = sum(counts.values())
    print(f"  {title}: {total}")
    for opcode, count in sorted(counts.items(), key=lambda item: (-item[1], item[0])):
        print(f"    {opcode}: {count}")


def print_width_counts(before: Counter[str], after: Counter[str]) -> None:
    shown_any = False
    for opcode in WIDTH_OPCODES:
        if before[opcode] == 0 and after[opcode] == 0:
            continue
        if not shown_any:
            print("  width ops before -> after:")
            shown_any = True
        print(f"    {opcode}: {before[opcode]} -> {after[opcode]}")


def print_report(
    path: Path, before: Counter[str], after: Counter[str], removed: Counter[str], added: Counter[str]
) -> None:
    total_removed = sum(removed.values())
    total_added = sum(added.values())
    print(path)
    if total_removed == 0 and total_added == 0:
        print("  changed: none")
        print_width_counts(before, after)
        return

    print_delta_block("removed", removed)
    print_delta_block("added", added)
    print_width_counts(before, after)


def main() -> int:
    repo_root = Path(__file__).resolve().parents[1]
    default_plugin = find_default_plugin(repo_root)
    default_opt = find_default_opt()

    parser = argparse.ArgumentParser(
        description=(
            "For each input .ll/.bc file, run width-opt and report how many "
            "instructions of each opcode disappeared."
        )
    )
    parser.add_argument(
        "--opt-bin",
        type=Path,
        default=default_opt,
        help=f"Path to opt (default: {default_opt}).",
    )
    parser.add_argument(
        "--plugin",
        type=Path,
        default=default_plugin,
        help="Path to libWidthOpt shared library.",
    )
    parser.add_argument(
        "inputs",
        nargs="+",
        type=Path,
        help="Input LLVM IR or bitcode files.",
    )
    args = parser.parse_args()

    opt_bin = args.opt_bin.resolve()
    plugin = args.plugin.resolve() if args.plugin is not None else None

    if not opt_bin.exists():
        print(f"error: opt not found: {opt_bin}", file=sys.stderr)
        return 1

    if plugin is None or not plugin.exists():
        print(
            "error: plugin not found; pass --plugin /path/to/libWidthOpt.so",
            file=sys.stderr,
        )
        return 1

    failures = 0
    for input_path in args.inputs:
        path = input_path.resolve()
        if not path.exists():
            print(f"error: input not found: {path}", file=sys.stderr)
            failures += 1
            continue

        original_proc = render_input_as_text(opt_bin, path)
        if original_proc.returncode != 0:
            print(format_failure("render", path, original_proc), file=sys.stderr)
            failures += 1
            continue

        optimized_proc = optimize_to_text(opt_bin, plugin, path)
        if optimized_proc.returncode != 0:
            print(format_failure("width-opt", path, optimized_proc), file=sys.stderr)
            failures += 1
            continue

        before = count_instructions(original_proc.stdout)
        after = count_instructions(optimized_proc.stdout)
        removed = Counter(
            {
                opcode: before[opcode] - after[opcode]
                for opcode in before.keys() | after.keys()
                if before[opcode] > after[opcode]
            }
        )
        added = Counter(
            {
                opcode: after[opcode] - before[opcode]
                for opcode in before.keys() | after.keys()
                if after[opcode] > before[opcode]
            }
        )
        print_report(path, before, after, removed, added)

    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
