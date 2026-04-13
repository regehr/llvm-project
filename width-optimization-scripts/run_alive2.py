#!/usr/bin/env python3
"""
Run Alive2 on every .ll file in llvm/test/Transforms/WidthOpt.

For each file:
  1. Run `opt -passes='width-opt' -S` to produce the optimized IR.
  2. Feed the (original, optimized) pair to alive-tv for verification.
  3. Report per-file results and a final summary.

Usage:
  python3 run_alive2.py [--jobs N] [--timeout MS] [--verbose]

Defaults: jobs=cpu_count, smt-timeout=10000ms, verbose=False
"""

import argparse
import os
import shutil
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

REPO_ROOT  = Path(__file__).parent.parent
TEST_DIR   = REPO_ROOT / "llvm/test/Transforms/WidthOpt"
OPT        = REPO_ROOT / "build/bin/opt"


def find_alive_tv() -> str:
    """Resolve alive-tv from PATH and exit with a readable error if missing."""
    alive_tv = shutil.which("alive-tv")
    if alive_tv is None:
        sys.exit("Could not find `alive-tv` in PATH")
    return alive_tv


def run(cmd, timeout_sec=120):
    """Run a command, return (returncode, stdout+stderr)."""
    try:
        r = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            timeout=timeout_sec,
        )
        return r.returncode, r.stdout.decode(errors="replace")
    except subprocess.TimeoutExpired:
        return -1, f"TIMED OUT after {timeout_sec}s"


def extra_alive_args(ll_path: Path) -> list:
    """
    Scan the file for a line of the form:
      ; ALIVE2-EXTRA-ARGS: <args>
    and return the extra arguments as a list, or [] if absent.
    """
    with ll_path.open() as f:
        for line in f:
            line = line.strip()
            if line.startswith("; ALIVE2-EXTRA-ARGS:"):
                return line.split(":", 1)[1].split()
            # Stop scanning once we leave the leading comment block
            if line and not line.startswith(";"):
                break
    return []


def verify_file(ll_path: Path, alive_tv: str, smt_timeout_ms: int, verbose: bool):
    """
    Optimise ll_path with width-opt, then verify with alive-tv.
    Returns (status, detail) where status is one of:
      "correct", "incorrect", "failed-to-prove", "opt-error", "alive-error", "timeout"
    """
    with tempfile.NamedTemporaryFile(suffix=".ll", delete=False) as tf:
        opt_out = tf.name

    try:
        # Step 1: optimise
        rc, out = run([str(OPT), "-passes=width-opt", "-S", str(ll_path),
                       "-o", opt_out])
        if rc != 0:
            return "opt-error", out

        # Step 2: verify
        rc, out = run(
            [alive_tv,
             "--disable-undef-input",
             f"--smt-to={smt_timeout_ms}",
             *extra_alive_args(ll_path),
             str(ll_path), opt_out],
        )

        if rc == -1:
            return "timeout", out

        # Parse the summary block; lines look like "  N keyword ..."
        lines = out.splitlines()
        summary = {}
        for line in lines:
            stripped = line.strip()
            for key in ("correct", "incorrect", "failed-to-prove", "Alive2 errors"):
                if key in stripped:
                    parts = stripped.split()
                    if parts:
                        try:
                            summary[key] = summary.get(key, 0) + int(parts[0])
                        except ValueError:
                            pass

        if not summary:
            return "alive-error", out

        if summary.get("incorrect", 0) > 0:
            return "incorrect", out
        if summary.get("Alive2 errors", 0) > 0:
            return "alive-error", out
        if summary.get("failed-to-prove", 0) > 0:
            return "failed-to-prove", out
        return "correct", out

    finally:
        try:
            os.unlink(opt_out)
        except OSError:
            pass


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--jobs", "-j", type=int,
                    default=os.cpu_count() or 4,
                    help="parallel workers (default: cpu count)")
    ap.add_argument("--timeout", type=int, default=10000,
                    help="SMT timeout per query in ms (default: 10000)")
    ap.add_argument("--verbose", "-v", action="store_true",
                    help="print alive-tv output for every file")
    args = ap.parse_args()
    alive_tv = find_alive_tv()

    ll_files = sorted(TEST_DIR.glob("*.ll"))
    if not ll_files:
        sys.exit(f"No .ll files found in {TEST_DIR}")

    print(f"Verifying {len(ll_files)} files with {args.jobs} workers "
          f"(SMT timeout {args.timeout}ms) …\n")

    counts = {k: 0 for k in
              ("correct", "incorrect", "failed-to-prove",
               "opt-error", "alive-error", "timeout")}
    results = {}

    with ThreadPoolExecutor(max_workers=args.jobs) as ex:
        futures = {
            ex.submit(verify_file, f, alive_tv, args.timeout, args.verbose): f
            for f in ll_files
        }
        for fut in as_completed(futures):
            f = futures[fut]
            status, detail = fut.result()
            counts[status] += 1
            results[f] = (status, detail)

            marker = "OK" if status == "correct" else status.upper()
            print(f"  [{marker:18s}]  {f.name}")
            if args.verbose or status != "correct":
                for line in detail.splitlines():
                    print(f"               {line}")
                print()

    # Final summary
    print()
    print("=" * 60)
    print(f"  Files checked       : {len(ll_files)}")
    print(f"  Correct             : {counts['correct']}")
    print(f"  Incorrect           : {counts['incorrect']}")
    print(f"  Failed-to-prove     : {counts['failed-to-prove']}")
    print(f"  opt errors          : {counts['opt-error']}")
    print(f"  Alive2 errors       : {counts['alive-error']}")
    print(f"  Timeouts            : {counts['timeout']}")
    print("=" * 60)

    bad = sum(v for k, v in counts.items() if k != "correct")
    sys.exit(0 if bad == 0 else 1)


if __name__ == "__main__":
    main()
