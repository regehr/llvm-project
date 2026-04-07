#!/usr/bin/env python3
"""
Read a list of CR log files from stdin, aggregate entries by instruction,
deduplicate, and write one output file per instruction to the given directory.

Input lines look like:
  mul i64 [8,9) [-2147483648,2147483648)   (binary)
  not i32 [0,5)                             (unary)

Output files are named <op>.log inside the output directory. Each line is:
  <count> <type> <arg1> [<arg2>]
sorted by descending count.
"""

import collections
import os
import re
import sys

# Matches the type field, e.g. "i64", which marks the boundary between op and args.
_TYPE_RE = re.compile(r'\bi\d+\b')


def main():
    if len(sys.argv) != 2:
        print(f"usage: {sys.argv[0]} <output-dir>", file=sys.stderr)
        sys.exit(1)

    out_dir = sys.argv[1]
    os.makedirs(out_dir, exist_ok=True)

    # op -> Counter of rest-of-line strings
    counts: dict[str, collections.Counter] = collections.defaultdict(collections.Counter)

    for line in sys.stdin:
        log_file = line.rstrip("\n")
        if not log_file:
            continue
        try:
            with open(log_file) as f:
                for entry in f:
                    entry = entry.rstrip("\n")
                    if not entry:
                        continue
                    # op is everything before the type field (e.g. "i64")
                    m = _TYPE_RE.search(entry)
                    if not m:
                        print(f"error: unparseable line in {log_file}: {entry!r}",
                              file=sys.stderr)
                        sys.exit(1)
                    op = entry[:m.start()].strip()
                    rest = entry[m.start():]
                    counts[op][rest] += 1
        except OSError as e:
            print(f"warning: {e}", file=sys.stderr)

    for op, counter in sorted(counts.items()):
        filename = op.replace(" ", "_") + ".log"
        out_path = os.path.join(out_dir, filename)
        with open(out_path, "w") as f:
            for rest, count in counter.most_common():
                f.write(f"{count} {rest}\n")


if __name__ == "__main__":
    main()
