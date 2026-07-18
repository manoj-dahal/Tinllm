#!/usr/bin/env python3
"""
TinLLM Benchmark Model Fixture Generator
Generates a lightweight synthetic SafeTensors checkpoint for testing local execution speed.
"""

import os
import sys
import json
import argparse

def create_fixture():
    parser = argparse.ArgumentParser(description="Generate synthetic TinLLM benchmark fixture model")
    parser.add_argument("--output", type=str, default="/tmp/tin_fixture", help="Output fixture path")
    args = parser.parse_args()

    os.makedirs(args.output, exist_ok=True)
    meta = {
        "model_type": "tinllm_benchmark_fixture",
        "hidden_size": 4096,
        "num_hidden_layers": 8,
        "num_experts": 16,
        "vocab_size": 32000
    }

    with open(os.path.join(args.output, "config.json"), "w") as f:
        json.dump(meta, f, indent=2)

    print(f"✓ Created benchmark synthetic model fixture at: {args.output}")

if __name__ == "__main__":
    create_fixture()
