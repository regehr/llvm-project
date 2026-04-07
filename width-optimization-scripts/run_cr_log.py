#!/usr/bin/env python3
"""
Read a list of LLVM IR files from stdin, run opt -O3 on each, and collect
ConstantRange transfer function logs. For foo.ll, the log goes to foo.log.
"""

import os
import subprocess
import sys

OPT = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                   "build", "bin", "opt")

for line in sys.stdin:
    ir_file = line.rstrip("\n")
    if not ir_file:
        continue
    log_file = os.path.splitext(ir_file)[0] + ".log"
    env = os.environ.copy()
    env["LLVM_CR_LOG"] = log_file
    subprocess.run(
        [OPT, "-O3", ir_file, "-o", "/dev/null"],
        env=env,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
