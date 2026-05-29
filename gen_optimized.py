#!/usr/bin/env python3
"""Run -O3 over the llvm-opt-benchmark suite with the locally built opt and
aggregate --stats-json counters.

Self-contained replacement for run_bench.sh + the benchmark repo's
scripts/gen_optimized.py. Point it at an llvm-opt-benchmark checkout; it scans
<bench_repo>/bench/<name>/original/*.ll, runs opt on each, writes the optimized
IR back into the sibling optimized/ dir (only when it changed), and sums the
stats into one JSON.

Examples:
  python3 gen_optimized.py ~/repos/llvm-opt-benchmark
  python3 gen_optimized.py ~/repos/llvm-opt-benchmark --filter cjson,coremark --stats outputs/run1.json
  python3 gen_optimized.py ~/repos/llvm-opt-benchmark --debug-logs outputs/run1_logs   # also dump [pNNN] logs
"""
import argparse
import json
import os
import subprocess
import sys
from multiprocessing import Pool
from pathlib import Path

try:
    import tqdm
except ImportError:
    tqdm = None

# Counters whose values vary run-to-run; excluded from the aggregate so repeated
# runs are comparable.
STATS_NONDETER_KEYS = {
    'dse.NumDomMemDefChecks', 'ir.NumInstrRenumberings',
    'basicaa.SearchTimes', 'aa.NumMayAlias',
    'capture-tracking.NumCaptured', 'aa.NumMustAlias',
    'memory-builtins.ObjectVisitorArgument', 'aa.NumNoAlias',
    'assume-queries.NumAssumeQueries', 'capture-tracking.NumNotCaptured',
    'ipt.NumInstScanned', 'simplifycfg.NumSimpl',
}

# Per-worker config, populated by _init_worker so it survives non-fork start
# methods too.
_OPT = None
_BENCH_DIR = None
_DEBUG_DIR = None


def _init_worker(opt: str, bench_dir: str, debug_dir: str | None) -> None:
    global _OPT, _BENCH_DIR, _DEBUG_DIR
    _OPT, _BENCH_DIR, _DEBUG_DIR = opt, bench_dir, debug_dir


def run_opt(task):
    input_file, output_file = task
    try:
        cmd = [
            _OPT, '-O3', '-disable-loop-unrolling',
            '-vectorize-loops=false', '-vectorize-slp=false', input_file, '-S',
        ]
        tmp_output = output_file + '.bench_tmp.ll'
        cmd += ['-o', tmp_output, '--stats', '--stats-json']
        if _DEBUG_DIR is not None:
            cmd += ['-debug-only=value-tracking-pattern']
        ret = subprocess.run(cmd, stdin=subprocess.DEVNULL,
                             capture_output=True, timeout=1200.0, env={})
        if ret.returncode != 0:
            return (input_file, 'fail', {}, ret.stderr.decode())

        err = ret.stderr.decode()
        stats = {}
        json_start = err.find('{')
        json_end = err.rfind('}')
        if json_start != -1 and json_end > json_start:
            try:
                stats = json.loads(err[json_start:json_end + 1])
            except json.JSONDecodeError:
                stats = {}

        if _DEBUG_DIR is not None:
            rel = os.path.relpath(input_file, _BENCH_DIR).replace('/original/', '/')
            log_path = (os.path.join(_DEBUG_DIR, rel[:-3] + '.log')
                        if rel.endswith('.ll')
                        else os.path.join(_DEBUG_DIR, rel + '.log'))
            os.makedirs(os.path.dirname(log_path), exist_ok=True)
            with open(log_path, 'w') as f:
                for line in err.splitlines():
                    if line.startswith('[p'):
                        f.write(line + '\n')

        diff_ret = subprocess.run(['diff', '-q', tmp_output, output_file],
                                  stdout=subprocess.DEVNULL,
                                  stderr=subprocess.DEVNULL)
        if diff_ret.returncode != 0:
            os.replace(tmp_output, output_file)
        else:
            os.remove(tmp_output)

        return (input_file, 'success', stats, '')
    except subprocess.TimeoutExpired:
        return (input_file, 'timeout', {}, '')
    except Exception as e:  # noqa: BLE001 - report any worker failure as a crash
        return (input_file, 'crash', {}, str(e))


def main() -> None:
    repo_root = Path(__file__).resolve().parent
    ap = argparse.ArgumentParser(
        description="Run -O3 over llvm-opt-benchmark and aggregate --stats-json.")
    ap.add_argument(
        'bench_repo', type=Path,
        help="Path to the llvm-opt-benchmark checkout; its bench/ subdir is "
        "scanned.")
    ap.add_argument(
        '--opt', type=Path, default=repo_root / 'build' / 'bin' / 'opt',
        help="opt binary to run. Default: %(default)s")
    ap.add_argument(
        '--filter', default='',
        help="Comma-separated benchmark dir names to include (default: all)")
    ap.add_argument(
        '--stats', type=Path, default=repo_root / 'outputs' / 'stats.json',
        help="Aggregated stats JSON output path. Default: %(default)s")
    ap.add_argument(
        '--debug-logs', type=Path, default=None,
        help="If set, also pass -debug-only=value-tracking-pattern and write "
        "per-input [pNNN] transformer-input logs here. Omit for stats only "
        "(no extra opt-side or IO cost).")
    ap.add_argument(
        '--jobs', type=int, default=os.cpu_count() or 1,
        help="Parallel worker processes. Default: %(default)s")
    args = ap.parse_args()

    bench_dir = args.bench_repo / 'bench'
    if not bench_dir.is_dir():
        sys.exit(f"error: {bench_dir} not found; pass the llvm-opt-benchmark path")
    if not os.access(args.opt, os.X_OK):
        sys.exit(f"error: opt binary {args.opt} not found or not executable")

    opt = str(args.opt)
    bench_dir = str(bench_dir)
    debug_dir = str(args.debug_logs) if args.debug_logs is not None else None
    filt = set(args.filter.split(',')) if args.filter else None

    work_list = []
    for name in sorted(os.listdir(bench_dir)):
        if filt and name not in filt:
            continue
        original_dir = os.path.join(bench_dir, name, 'original')
        if not os.path.isdir(original_dir):
            continue
        optimized_dir = os.path.join(bench_dir, name, 'optimized')
        os.makedirs(optimized_dir, exist_ok=True)
        for f in os.listdir(original_dir):
            if f.endswith('.ll'):
                work_list.append((os.path.join(original_dir, f),
                                  os.path.join(optimized_dir, f)))

    if debug_dir is not None:
        if os.path.exists(debug_dir) and not os.path.isdir(debug_dir):
            sys.exit(f"error: --debug-logs {debug_dir!r} exists but is not a directory")
        os.makedirs(debug_dir, exist_ok=True)

    print("total items: ", len(work_list))
    print("threads: ", args.jobs)

    stats_out = str(args.stats)
    stats_out_dir = os.path.dirname(os.path.abspath(stats_out))
    os.makedirs(stats_out_dir, exist_ok=True)
    test_log_path = os.path.join(stats_out_dir, 'test.log')

    stats_acc: dict[str, float] = {}
    fail = False

    pool = Pool(processes=args.jobs, initializer=_init_worker,
                initargs=(opt, bench_dir, debug_dir))
    results = pool.imap_unordered(run_opt, work_list)
    if tqdm is not None:
        results = tqdm.tqdm(results, total=len(work_list),
                            miniters=max(1, len(work_list) // 200))

    with open(test_log_path, 'w') as log:
        for input_file, status, stats, err in results:
            rel = os.path.relpath(input_file, bench_dir).replace('/original/', '/')
            if status != 'success':
                msg = f"{rel} {status}"
                (tqdm.tqdm.write if tqdm is not None else print)(msg)
                if err:
                    print(err, file=sys.stderr)
                log.write(msg + '\n')
                fail = True
            else:
                for k, v in stats.items():
                    if k in STATS_NONDETER_KEYS:
                        continue
                    stats_acc[k] = stats_acc.get(k, 0) + v

    pool.close()
    pool.join()

    with open(stats_out, 'w') as f:
        json.dump(stats_acc, f, indent=2, sort_keys=True)
    print(f"aggregated stats written to {stats_out} ({len(stats_acc)} keys)")

    sys.exit(1 if fail else 0)


if __name__ == '__main__':
    main()
