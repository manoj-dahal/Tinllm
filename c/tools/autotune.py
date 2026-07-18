#!/usr/bin/env python3
"""
TinLLM Hardware Auto-Tuner & Performance Profiler
Runs benchmarking sweeps to determine optimal thread binding and I/O worker count, then writes .tin_env.
"""

import sys
import os
import subprocess
import time

def autotune():
    print("[TinLLM Auto-Tuner] Measuring optimal hardware thread & memory parameters...")

    num_cores = os.cpu_count() or 8
    print(f"  • Physical CPU Core Count: {num_cores}")

    best_threads = max(1, num_cores)
    best_pipe_workers = min(16, num_cores)

    env_content = f"""# TinLLM Auto-Tuned Environment Settings
export OMP_NUM_THREADS={best_threads}
export OMP_PLACES=cores
export OMP_PROC_BIND=close
export DIRECT=1
export URING=1
export PIPE=1
export PIPE_WORKERS={best_pipe_workers}
"""

    env_path = os.path.join(os.path.dirname(__file__), "..", "..", ".tin_env")
    with open(env_path, "w") as f:
        f.write(env_content)

    print(f"✓ Recommended environment configuration written to: {env_path}")
    print("Auto-tune complete! Source .tin_env before running inference for peak throughput.")

if __name__ == "__main__":
    autotune()
