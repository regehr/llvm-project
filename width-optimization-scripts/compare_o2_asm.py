#!/usr/bin/env python3

import argparse
import subprocess
import sys
from pathlib import Path


OLD_BIN = Path("/home/regehr/tmp/llvm-project-regehr-2/build/bin")
NEW_BIN = Path("/home/regehr/tmp/llvm-project-regehr/build/bin")

BUILDS = {
    "old": OLD_BIN,
    "new": NEW_BIN,
}

TARGETS = {
    "aarch64": "aarch64-unknown-linux-gnu",
    "riscv64": "riscv64-unknown-linux-gnu",
    "x86_64": "x86_64-unknown-linux-gnu",
}


def run(cmd: list[str]) -> None:
    print("+", " ".join(str(part) for part in cmd), flush=True)
    subprocess.run(cmd, check=True)


def input_stem(path: Path) -> str:
    if path.suffix in {".ll", ".bc"}:
        return path.stem
    return path.name


def ensure_tools_exist() -> int:
    missing = []
    for bindir in BUILDS.values():
        for tool in ("opt", "llc"):
            tool_path = bindir / tool
            if not tool_path.is_file():
                missing.append(tool_path)

    if not missing:
        return 0

    for tool_path in missing:
        print(f"missing tool: {tool_path}", file=sys.stderr)
    return 1


def optimize_ir(src: Path, tag: str, bindir: Path, out_dir: Path) -> Path:
    out_path = out_dir / f"{input_stem(src)}.{tag}.O2.ll"
    run([str(bindir / "opt"), "-S", "-O2", str(src), "-o", str(out_path)])
    return out_path


def lower_to_asm(opt_ir: Path, bindir: Path) -> None:
    asm_base = opt_ir.with_suffix("").name
    for arch, triple in TARGETS.items():
        asm_path = opt_ir.parent / f"{asm_base}.{arch}.s"
        run(
            [
                str(bindir / "llc"),
                "-mtriple",
                triple,
                "-filetype=asm",
                str(opt_ir),
                "-o",
                str(asm_path),
            ]
        )


def process_file(src: Path, out_dir: Path) -> None:
    if not src.is_file():
        raise FileNotFoundError(f"input file not found: {src}")

    print(f"==> Processing {src}")
    optimized = {}
    for tag, bindir in BUILDS.items():
        optimized[tag] = (optimize_ir(src, tag, bindir, out_dir), bindir)

    for opt_ir, bindir in optimized.values():
        lower_to_asm(opt_ir, bindir)


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Optimize each input LLVM IR file with two LLVM builds using -O2, "
            "then lower both optimized IR files to AArch64, RISC-V 64, and x86-64 assembly."
        )
    )
    parser.add_argument("inputs", nargs="+", help="LLVM IR files to process")
    args = parser.parse_args()

    if ensure_tools_exist() != 0:
        return 1

    try:
        out_dir = Path.cwd()
        for name in args.inputs:
            process_file(Path(name).resolve(), out_dir)
    except subprocess.CalledProcessError as err:
        return err.returncode
    except Exception as err:
        print(err, file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
