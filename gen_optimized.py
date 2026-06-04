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
"""
import argparse
import collections
import glob
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

# Share the id<->expression codec with the table_builder tooling.
sys.path.insert(0, str(Path(__file__).resolve().parent / "table_builder"))
from util import decode_id_to_expr  # noqa: E402

# Where generate_pattern_dispatcher.py writes the per-pattern <id>.inc files.
# Their stems are the only record of the routing-index -> expression mapping, so
# merge_histograms reads them to name the per-pattern histogram TSVs.
PATTERNS_DIR = Path(__file__).resolve().parent / "llvm/lib/Analysis/Generated/patterns"

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
_HIST_DIR = None


def _init_worker(opt: str, bench_dir: str, hist_dir: str | None) -> None:
    global _OPT, _BENCH_DIR, _HIST_DIR
    _OPT, _BENCH_DIR, _HIST_DIR = opt, bench_dir, hist_dir


def _rel_stem(input_file: str) -> str:
    """benchmark-relative path with /original/ folded out and no .ll suffix."""
    rel = os.path.relpath(input_file, _BENCH_DIR).replace('/original/', '/')
    return rel[:-3] if rel.endswith('.ll') else rel


def run_opt(task):
    input_file, output_file = task
    try:
        cmd = [
            _OPT, '-O3', '-disable-loop-unrolling',
            '-vectorize-loops=false', '-vectorize-slp=false', input_file, '-S',
        ]
        tmp_output = output_file + '.bench_tmp.ll'
        cmd += ['-o', tmp_output, '--stats', '--stats-json']
        if _HIST_DIR is not None:
            # opt writes the per-input histogram itself (no stderr/pipe/parse).
            hist_path = os.path.join(_HIST_DIR, 'shards', _rel_stem(input_file) + '.hist')
            os.makedirs(os.path.dirname(hist_path), exist_ok=True)
            cmd += [f'-value-tracking-pattern-histogram={hist_path}']
        ret = subprocess.run(cmd, stdin=subprocess.DEVNULL,
                             capture_output=True, timeout=600.0, env={})
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
        '--pattern-hist', type=Path, default=None,
        help="If set, have opt accumulate a per-input histogram of pattern "
        "inputs (id + operand known bits, summed bits_added/conflict) and "
        "write shards under <dir>/shards/, then merge them into per-pattern "
        "<dir>/pattern_<id>.tsv. Avoids the giant raw [pNNN] logs entirely.")
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
    hist_dir = str(args.pattern_hist) if args.pattern_hist is not None else None
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

    if hist_dir is not None:
        if os.path.exists(hist_dir) and not os.path.isdir(hist_dir):
            sys.exit(f"error: --pattern-hist {hist_dir!r} exists but is not a directory")
        os.makedirs(os.path.join(hist_dir, 'shards'), exist_ok=True)

    print("total items: ", len(work_list))
    print("threads: ", args.jobs)

    stats_out = str(args.stats)
    stats_out_dir = os.path.dirname(os.path.abspath(stats_out))
    os.makedirs(stats_out_dir, exist_ok=True)
    test_log_path = os.path.join(stats_out_dir, 'test.log')

    stats_acc: dict[str, float] = {}
    fail = False

    pool = Pool(processes=args.jobs, initializer=_init_worker,
                initargs=(opt, bench_dir, hist_dir))
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

    if hist_dir is not None:
        merge_histograms(hist_dir)

    sys.exit(1 if fail else 0)


def load_pattern_names() -> dict[int, str]:
    """Reconstruct the generator's routing-index -> pattern-id mapping.

    The ids are the <id>.inc stems in PATTERNS_DIR (the dispatcher's generated
    patterns/ tree); the routing index is each id's position in sorted order,
    exactly as generate_pattern_dispatcher.py assigns it. Returns an empty map if
    that directory is absent, so histograms fall back to numeric ids.
    """
    ids = sorted(p.stem for p in PATTERNS_DIR.glob("*.inc"))
    return dict(enumerate(ids))


def pattern_yaml_header(op: str, arity: int,
                        bw_counts: dict[int, int]) -> str:
    """The '# ---'-delimited YAML preamble used by results/tsv/*/pattern_*.tsv.

    `op` is the bare instruction name (no path). The per-bitwidth distinct-row
    counts go in hbw as [bw, count, 0] (mbw/lbw left empty), per the conversion
    mbw:[x, y] -> hbw:[x, y, 0].
    """
    lines = ['# ---', '# domain: KnownBits', f'# op: {op}', f'# arity: {arity}',
             '# seed: null', '# lbw: []', '# mbw: []']
    if bw_counts:
        lines.append('# hbw:')
        for bw in sorted(bw_counts):
            lines.append(f'# - [{bw}, {bw_counts[bw]}, 0]')
    else:
        lines.append('# hbw: []')
    lines.append('# ---')
    return '\n'.join(lines) + '\n'


def merge_histograms(hist_dir: str) -> None:
    """Merge per-input .hist shards into ranked per-pattern TSVs.

    Each shard line is: <id> <arg_0> <arg_1> ... <count> <bits_added> <conflict>
    (tab-separated). We sum count/bits_added/conflict across shards per
    (id, args), then per id emit pattern_<name>.tsv ranked by count desc.
    """
    names = load_pattern_names()
    # id -> {args-tuple -> [count, bits_added, conflict]}
    acc: dict[int, dict[tuple, list]] = collections.defaultdict(dict)
    shards = glob.glob(os.path.join(hist_dir, 'shards', '**', '*.hist'),
                       recursive=True)
    for shard in shards:
        with open(shard) as f:
            for line in f:
                parts = line.rstrip('\n').split('\t')
                if len(parts) < 4:
                    continue
                pid = int(parts[0])
                count, bits_added, conflict = (int(parts[-3]), int(parts[-2]),
                                               int(parts[-1]))
                args = tuple(parts[1:-3])
                row = acc[pid].setdefault(args, [0, 0, 0])
                row[0] += count
                row[1] += bits_added
                row[2] += conflict

    n_tables = 0
    for pid, rows in acc.items():
        # The filename is the bare pattern id (no `pattern_` prefix); the YAML
        # header's `op` field carries the decoded expression for readability.
        name = names.get(pid, f'{pid:03d}')
        try:
            op = decode_id_to_expr(name)
        except ValueError:
            op = name  # numeric fallback id (no PATTERNS_DIR): not decodable.
        arity = max(len(a) for a in rows) if rows else 0
        ranked = sorted(rows.items(), key=lambda kv: -kv[1][0])
        # distinct-row count per bitwidth (= ternary string length) for hbw.
        bw_counts = collections.Counter(len(a[0]) for a in rows if a)
        out_path = os.path.join(hist_dir, f'{name}.tsv')
        with open(out_path, 'w') as f:
            f.write(pattern_yaml_header(op, arity, bw_counts))
            header = (['bw', 'rank', 'count'] + [f'arg_{i}' for i in range(arity)]
                      + ['bits_added', 'conflict'])
            f.write('\t'.join(header) + '\n')
            for rank, (args, (count, bits_added, conflict)) in enumerate(ranked, 1):
                bw = len(args[0]) if args else 0
                f.write('\t'.join([str(bw), str(rank), str(count), *args,
                                   str(bits_added), str(conflict)]) + '\n')
        n_tables += 1
    print(f"merged {len(shards)} shards -> {n_tables} pattern TSVs in {hist_dir}")


if __name__ == '__main__':
    main()
