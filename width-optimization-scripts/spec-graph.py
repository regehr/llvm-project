#!/usr/bin/env python3
"""
spec-graph.py <baseline_run> <new_run>

Produces a grouped bar chart showing run times (lower is better) for
each SPECrate 2017 int benchmark (base metric), comparing a new
compiler run against a baseline.

Example: spec-graph.py 001 002
"""

import sys
import argparse
import csv
import glob
import os
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import numpy as np


def find_csv(run_id):
    """Return the path to the result CSV for a given run ID, avoiding .N.csv files."""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    pattern = os.path.join(script_dir, f"CPU2017.{run_id}.intrate.csv")
    matches = [p for p in glob.glob(pattern)
               if not any(c.isdigit() for c in os.path.basename(p).split(".intrate.")[-1].replace(".csv", ""))]
    if not matches:
        sys.exit(f"Error: no result CSV found for run {run_id} (pattern: {pattern})")
    if len(matches) > 1:
        sys.exit(f"Error: multiple CSVs matched for run {run_id}: {matches}")
    return matches[0]


def parse_results(path):
    """
    Parse a SPEC CSV result file.
    Returns (benchmarks, overall_base_rate) where benchmarks is a dict
    mapping benchmark name -> Est. Base Rate (float) from the Selected Results Table.
    """
    benchmarks = {}
    overall_base = None

    with open(path, newline="", encoding="utf-8") as f:
        reader = csv.reader(f)
        rows = list(reader)

    # Find "Selected Results Table" section
    sel_start = None
    for i, row in enumerate(rows):
        if row and row[0].strip('"') == "Selected Results Table":
            sel_start = i
            break
    if sel_start is None:
        sys.exit(f"Error: 'Selected Results Table' not found in {path}")

    # Skip the section header line and the blank line after it, then the column header
    # Structure: sel_start -> blank -> column header -> data rows -> blank
    data_start = sel_start + 3  # skip section title, blank, column header
    for row in rows[data_start:]:
        if not row or row[0].strip() == "":
            break
        name = row[0].strip()
        if name.startswith("SPECrate"):
            break
        try:
            base_time = float(row[2])
        except (IndexError, ValueError):
            continue
        benchmarks[name] = base_time

    # Find SPECrate2017_int_base summary line
    for row in rows:
        if row and row[0].strip().startswith("SPECrate2017_int_base"):
            # Format: SPECrate2017_int_base,<value>,,<value>
            for cell in row[1:]:
                cell = cell.strip()
                if cell:
                    try:
                        overall_base = float(cell)
                        break
                    except ValueError:
                        continue
            break

    return benchmarks, overall_base


def main():
    parser = argparse.ArgumentParser(
        description="Graph SPECrate 2017 int base run times: new compiler vs baseline.")
    parser.add_argument("baseline_run", help="Run number for the baseline (e.g. 001)")
    parser.add_argument("new_run", help="Run number for the new compiler (e.g. 002)")
    parser.add_argument("--name", default=None,
                        help="Name for the new compiler (default: run number)")
    parser.add_argument("--baseline-name", default=None,
                        help="Name for the baseline compiler (default: run number)")
    args = parser.parse_args()

    baseline_id, new_id = args.baseline_run, args.new_run
    new_label = args.name if args.name else f"run {new_id}"
    baseline_label = args.baseline_name if args.baseline_name else f"run {baseline_id}"
    baseline_csv = find_csv(baseline_id)
    new_csv = find_csv(new_id)

    baseline_benches, baseline_overall = parse_results(baseline_csv)
    new_benches, new_overall = parse_results(new_csv)

    # Find common benchmarks, preserving SPEC suite order
    all_names = [b for b in baseline_benches if b in new_benches]
    if not all_names:
        sys.exit("Error: no common benchmarks found between the two runs.")

    # Strip numeric prefix for cleaner labels (e.g. "500.perlbench_r" -> "perlbench_r")
    labels = [b.split(".", 1)[1] if "." in b else b for b in all_names]

    baseline_times = [baseline_benches[b] for b in all_names]
    new_times = [new_benches[b] for b in all_names]

    bar_width = 0.35
    x = np.arange(len(labels))

    fig, ax = plt.subplots(figsize=(13, 6))
    bars_base = ax.bar(x - bar_width / 2, baseline_times, bar_width,
                       label=baseline_label,
                       color="#1f77b4", edgecolor="black", linewidth=0.6)
    bars_new = ax.bar(x + bar_width / 2, new_times, bar_width,
                      label=new_label,
                      color="#ff7f0e", edgecolor="black", linewidth=0.6)

    ax.set_xticks(x)
    ax.set_xticklabels(labels, rotation=30, ha="right", fontsize=9)
    ax.set_ylabel("Est. Base Run Time (s)  —  lower is better")
    ax.legend(fontsize=9)

    # Annotate new bars with % change relative to baseline
    max_time = max(max(baseline_times), max(new_times))
    for bbar, nbar, bt, nt in zip(bars_base, bars_new, baseline_times, new_times):
        pct = (nt - bt) / bt * 100
        color = "#d62728" if pct > 0 else "#2ca02c"  # red=slower, green=faster
        ax.text(
            nbar.get_x() + nbar.get_width() / 2,
            nt + max_time * 0.01,
            f"{pct:+.1f}%",
            ha="center", va="bottom", fontsize=7, color=color, fontweight="bold",
        )

    overall_str = ""
    if baseline_overall is not None and new_overall is not None:
        overall_pct = (new_overall - baseline_overall) / baseline_overall * 100
        overall_str = (
            f"  |  SPECrate2017_int_base: {baseline_overall:.3f} → {new_overall:.3f}"
            f" ({overall_pct:+.1f}%)"
        )

    ax.set_title(
        f"SPECrate 2017 Int Base run times: {new_label} vs {baseline_label}{overall_str}",
        fontsize=10,
    )

    plt.tight_layout()
    out_path = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                            f"spec_comparison_{baseline_id}_vs_{new_id}.png")
    plt.savefig(out_path, dpi=150)
    print(f"Graph saved to: {out_path}")
    plt.show()


if __name__ == "__main__":
    main()
