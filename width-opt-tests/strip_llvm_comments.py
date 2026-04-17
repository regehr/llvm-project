#!/usr/bin/env python3

from __future__ import annotations

import argparse
import os
import shutil
import tempfile
from pathlib import Path


def strip_comments(path: Path) -> None:
    fd, temp_path_str = tempfile.mkstemp(
        dir=path.parent,
        prefix=f".{path.name}.",
        suffix=".tmp",
        text=True,
    )
    temp_path = Path(temp_path_str)

    try:
        with os.fdopen(fd, "w", encoding="utf-8", newline="") as dst:
            with path.open("r", encoding="utf-8", newline="") as src:
                for line in src:
                    if not line.startswith(";"):
                        dst.write(line)

        shutil.copystat(path, temp_path, follow_symlinks=True)
        os.replace(temp_path, path)
    except Exception:
        try:
            temp_path.unlink()
        except FileNotFoundError:
            pass
        raise


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Remove full-line LLVM IR comments from files in place."
    )
    parser.add_argument("files", nargs="+", type=Path, help="LLVM IR files to rewrite")
    args = parser.parse_args()

    for path in args.files:
        strip_comments(path)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
