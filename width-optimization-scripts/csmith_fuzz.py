#!/usr/bin/env python3
"""
Differential testing: compare gcc -O3 vs clang -O3 on random Csmith programs.
Stops on checksum mismatch or compiler crash; runs forever otherwise.
"""

import multiprocessing
import os
import subprocess
import sys
import tempfile
from pathlib import Path

CSMITH     = "/home/regehr/csmith/build/src/csmith"
GCC        = "gcc"
CLANG      = "/home/regehr/tmp/llvm-project-regehr/build/bin/clang"
CSMITH_INC = [
    "/home/regehr/csmith/runtime",
    "/home/regehr/csmith/build/runtime",
]

COMPILE_FLAGS = ["-w", "-O3"] + [f"-I{d}" for d in CSMITH_INC]


def run(cmd, **kwargs):
    try:
        return subprocess.run(cmd, capture_output=True, text=True, **kwargs)
    except subprocess.TimeoutExpired:
        return None


def extract_checksum(output: str):
    for line in output.splitlines():
        if line.startswith("checksum"):
            return line.strip()
    return None


def worker(worker_id, stop_event, result_queue, print_lock):
    while not stop_event.is_set():
        with tempfile.TemporaryDirectory() as tmp:
            src       = os.path.join(tmp, "test.c")
            bin_gcc   = os.path.join(tmp, "out_gcc")
            bin_clang = os.path.join(tmp, "out_clang")

            # Generate
            gen = run([CSMITH], timeout=30)
            if gen is None or gen.returncode != 0:
                continue
            with open(src, "w") as f:
                f.write(gen.stdout)

            # Compile with gcc
            r_gcc = run([GCC] + COMPILE_FLAGS + [src, "-o", bin_gcc], timeout=60)
            if r_gcc is None:
                continue
            if r_gcc.returncode != 0:
                result_queue.put(("compiler_crash", "gcc", src, r_gcc.stderr))
                return

            # Compile with clang
            r_clang = run([CLANG] + COMPILE_FLAGS + [src, "-o", bin_clang], timeout=60)
            if r_clang is None:
                continue
            if r_clang.returncode != 0:
                result_queue.put(("compiler_crash", "clang", src, r_clang.stderr))
                return

            # Run both; timeout means the program ran forever — skip this test case
            r_run_gcc   = run([bin_gcc],   timeout=10)
            r_run_clang = run([bin_clang], timeout=10)
            if r_run_gcc is None or r_run_clang is None:
                continue

            cs_gcc   = extract_checksum(r_run_gcc.stdout)
            cs_clang = extract_checksum(r_run_clang.stdout)

            if cs_gcc != cs_clang:
                result_queue.put(("mismatch", src, cs_gcc, cs_clang, gen.stdout))
                return

            with print_lock:
                print(f"worker {worker_id:3d}: gcc={cs_gcc}  clang={cs_clang}", flush=True)


def main():
    ncpus = multiprocessing.cpu_count()
    print(f"Starting {ncpus} workers (gcc={GCC}, clang={CLANG})")
    sys.stdout.flush()

    stop_event = multiprocessing.Event()
    result_queue = multiprocessing.Queue()
    print_lock = multiprocessing.Lock()

    workers = [
        multiprocessing.Process(target=worker, args=(i, stop_event, result_queue, print_lock), daemon=True)
        for i in range(ncpus)
    ]
    for w in workers:
        w.start()

    try:
        result = result_queue.get()  # blocks until a worker reports a problem
    except KeyboardInterrupt:
        print("\nInterrupted.")
        stop_event.set()
        for w in workers:
            w.join(timeout=3)
        return 0

    stop_event.set()

    kind = result[0]
    if kind == "compiler_crash":
        _, compiler, src_path, stderr = result
        print(f"\nCOMPILER CRASH: {compiler}")
        print(f"stderr:\n{stderr}")
        save = f"/tmp/csmith_crash_{compiler}.c"
        with open(save, "w") as f:
            # src_path may be in a tempdir that still exists; just re-read if possible
            try:
                with open(src_path) as s:
                    f.write(s.read())
                print(f"Source saved to {save}")
            except OSError:
                pass

    elif kind == "mismatch":
        _, src_path, cs_gcc, cs_clang, source = result
        print(f"\nCHECKSUM MISMATCH")
        print(f"  gcc   : {cs_gcc}")
        print(f"  clang : {cs_clang}")
        save = "/tmp/csmith_mismatch.c"
        with open(save, "w") as f:
            f.write(source)
        print(f"Source saved to {save}")

    for w in workers:
        w.join(timeout=3)
    return 1


if __name__ == "__main__":
    sys.exit(main())
