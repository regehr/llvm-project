#!/usr/bin/env python3
"""Smoke test for the stub-transfer -> dispatcher pipeline.

Drives the seed-list fixture (top_10_pattern.tsv) through the two scripts that
form a runnable pipeline and just checks both run without error:

    top_10_pattern.tsv
        -> build_stub_xfer.py             (patterns.json + stub pattern_*.inc)
        -> generate_pattern_dispatcher.py (KnownBitsPatternDispatch.inc + patterns/)

Fully self-contained: reads only the checked-in fixture and writes only into a
temp dir (--generated-dir is redirected so the real Generated/ tree is never
touched). Nothing under results/ is used.

Run directly:  python3 table_builder/tests/test_stub_xfer_dispatch.py
Or via pytest: pytest table_builder/tests/test_stub_xfer_dispatch.py
"""
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent          # table_builder/tests
TABLE_BUILDER = HERE.parent                      # table_builder
REPO_ROOT = TABLE_BUILDER.parent                 # repo root

FIXTURE = HERE / "top_10_pattern.tsv"
BUILD_STUB_XFER = TABLE_BUILDER / "build_stub_xfer.py"


def _find_dispatcher() -> Path:
    """generate_pattern_dispatcher.py has lived at both the repo root and under
    table_builder/; accept either so the test survives the move."""
    for cand in (TABLE_BUILDER / "generate_pattern_dispatcher.py",
                 REPO_ROOT / "generate_pattern_dispatcher.py"):
        if cand.exists():
            return cand
    raise FileNotFoundError("generate_pattern_dispatcher.py not found")


def _run(script: Path, *argv: object) -> None:
    """Run a helper script; raise with captured output if it errors."""
    proc = subprocess.run(
        [sys.executable, str(script), *map(str, argv)],
        cwd=REPO_ROOT, capture_output=True, text=True,
    )
    if proc.returncode != 0:
        raise AssertionError(
            f"{script.name} exited {proc.returncode}\n"
            f"--- stdout ---\n{proc.stdout}\n--- stderr ---\n{proc.stderr}"
        )


def run_pipeline(workdir: Path) -> None:
    folder = workdir / "folder"      # the transformer folder the dispatcher reads

    # 1) seed TSV -> stub transformer. build_stub_xfer writes a well-formed
    #    folder directly: patterns.json at the root and inc/pattern_*.inc.
    _run(BUILD_STUB_XFER, "--tsv", FIXTURE, "--out-dir", folder)

    # 2) transformer folder -> dispatcher. Redirect --generated-dir into the temp
    #    tree so the real llvm/lib/Analysis/Generated is never written.
    _run(_find_dispatcher(), folder, "--generated-dir", workdir / "gen")


def test_stub_xfer_feeds_dispatcher() -> None:
    with tempfile.TemporaryDirectory() as td:
        run_pipeline(Path(td))


if __name__ == "__main__":
    with tempfile.TemporaryDirectory() as td:
        run_pipeline(Path(td))
    print("PASS: stub_xfer -> dispatcher pipeline ran without error")
