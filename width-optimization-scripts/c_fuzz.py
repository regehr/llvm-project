#!/usr/bin/env python3
"""
Differential testing: compare gcc -O3 vs clang -O3 on random Csmith programs.
Stops on checksum mismatch or compiler crash; runs forever otherwise.
"""

import argparse
import multiprocessing
import os
import signal
import subprocess
import sys
import tempfile
from pathlib import Path

CSMITH     = "/home/regehr/csmith/build/src/csmith"
YARPGEN    = "/home/regehr/yarpgen/build/yarpgen"
GCC        = "gcc"
CLANG      = "/home/regehr/tmp/llvm-project-regehr/build/bin/clang"
CSMITH_INC = [
    "/home/regehr/csmith/runtime",
    "/home/regehr/csmith/build/runtime",
]

CSMITH_COMPILE_FLAGS = ["-w", "-O3", "-mcmodel=large"] + [f"-I{d}" for d in CSMITH_INC]
YARPGEN_COMPILE_FLAGS = ["-w", "-O3", "-mcmodel=large"]


def run(cmd, timeout=None, **kwargs):
    try:
        proc = subprocess.Popen(
            cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
            start_new_session=True, **kwargs
        )
        try:
            stdout, stderr = proc.communicate(timeout=timeout)
            return subprocess.CompletedProcess(cmd, proc.returncode, stdout, stderr)
        except subprocess.TimeoutExpired:
            os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
            proc.communicate()
            return None
    except Exception:
        return None


def extract_checksum(output: str):
    for line in output.splitlines():
        if line.startswith("checksum"):
            return line.strip()
    return None


def save_slow(source: str, compiler: str, save: bool):
    if save:
        fd, path = tempfile.mkstemp(prefix="slow_compile_", suffix=".c", dir=os.getcwd())
        with os.fdopen(fd, "w") as f:
            f.write(source)
        print(f"slow {compiler} compile saved to {path}", flush=True)
    else:
        print(f"slow {compiler} compile (skipped saving; use --save-slow to save)", flush=True)


def worker_csmith(worker_id, stop_event, result_queue, print_lock, counter, counter_lock, save_slow_files):
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
            r_gcc = run([GCC] + CSMITH_COMPILE_FLAGS + [src, "-o", bin_gcc], timeout=180)
            if r_gcc is None:
                save_slow(gen.stdout, "gcc", save_slow_files)
                continue
            if r_gcc.returncode != 0:
                result_queue.put(("compiler_crash", "gcc", src, r_gcc.stderr))
                return

            # Compile with clang
            r_clang = run([CLANG] + CSMITH_COMPILE_FLAGS + [src, "-o", bin_clang], timeout=180)
            if r_clang is None:
                save_slow(gen.stdout, "clang", save_slow_files)
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

            with counter_lock:
                worker_id = counter.value
                counter.value += 1
            with print_lock:
                print(f"worker {worker_id:3d}: gcc={cs_gcc}  clang={cs_clang}", flush=True)


def worker_yarpgen(worker_id, stop_event, result_queue, print_lock, counter, counter_lock, save_slow_files):
    while not stop_event.is_set():
        with tempfile.TemporaryDirectory() as tmp:
            gen_dir   = os.path.join(tmp, "gen")
            bin_gcc   = os.path.join(tmp, "out_gcc")
            bin_clang = os.path.join(tmp, "out_clang")
            os.makedirs(gen_dir)

            # Generate
            gen = run([YARPGEN, "--std=c", "-o", gen_dir], timeout=30)
            if gen is None or gen.returncode != 0:
                continue

            src_files = sorted(str(p) for p in Path(gen_dir).glob("*.c"))
            if not src_files:
                continue

            # Read source for saving on crash/mismatch
            source = "\n".join(open(f).read() for f in src_files)

            # Compile with gcc
            r_gcc = run([GCC] + YARPGEN_COMPILE_FLAGS + src_files + ["-o", bin_gcc], timeout=180)
            if r_gcc is None:
                save_slow(source, "gcc", save_slow_files)
                continue
            if r_gcc.returncode != 0:
                result_queue.put(("compiler_crash", "gcc", src_files[0], r_gcc.stderr))
                return

            # Compile with clang
            r_clang = run([CLANG] + YARPGEN_COMPILE_FLAGS + src_files + ["-o", bin_clang], timeout=180)
            if r_clang is None:
                save_slow(source, "clang", save_slow_files)
                continue
            if r_clang.returncode != 0:
                result_queue.put(("compiler_crash", "clang", src_files[0], r_clang.stderr))
                return

            # Run both; timeout means the program ran forever — skip this test case
            r_run_gcc   = run([bin_gcc],   timeout=10)
            r_run_clang = run([bin_clang], timeout=10)
            if r_run_gcc is None or r_run_clang is None:
                continue

            cs_gcc   = r_run_gcc.stdout.strip()
            cs_clang = r_run_clang.stdout.strip()

            if cs_gcc != cs_clang:
                result_queue.put(("mismatch", src_files[0], cs_gcc, cs_clang, source))
                return

            with counter_lock:
                worker_id = counter.value
                counter.value += 1
            with print_lock:
                print(f"worker {worker_id:3d}: gcc={cs_gcc}  clang={cs_clang}", flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    mode_group = parser.add_mutually_exclusive_group(required=True)
    mode_group.add_argument("--csmith", action="store_true",
                            help="Generate test programs using Csmith")
    mode_group.add_argument("--yarpgen-v2", action="store_true",
                            help="Generate test programs using YARPGen v2")
    parser.add_argument("--save-slow", action="store_true",
                        help="Save programs that time out during compilation to disk")
    args = parser.parse_args()

    if args.yarpgen_v2:
        worker_fn = worker_yarpgen
        gen_label = f"yarpgen={YARPGEN}"
    else:
        worker_fn = worker_csmith
        gen_label = f"csmith={CSMITH}"

    ncpus = multiprocessing.cpu_count()
    print(f"Starting {ncpus} workers ({gen_label}, gcc={GCC}, clang={CLANG})")
    sys.stdout.flush()

    stop_event = multiprocessing.Event()
    result_queue = multiprocessing.Queue()
    print_lock = multiprocessing.Lock()
    counter = multiprocessing.Value("i", 0)
    counter_lock = multiprocessing.Lock()

    workers = [
        multiprocessing.Process(target=worker_fn, args=(i, stop_event, result_queue, print_lock, counter, counter_lock, args.save_slow), daemon=True)
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
