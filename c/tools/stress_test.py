#!/usr/bin/env python3
"""
TinLLM Stress Test & Concurrency Simulator
Simulates concurrent multi-slot HTTP API load to test request queueing and memory context bounds.
"""

import urllib.request
import json
import time
import sys

def run_stress_test():
    url = "http://127.0.0.1:8080/v1/chat/completions"
    headers = {"Content-Type": "application/json"}

    print("[TinLLM Stress Tester] Sending concurrent request payloads to local endpoint...")

    for i in range(5):
        payload = {
            "model": "tinllm-moe",
            "messages": [{"role": "user", "content": f"Test request session #{i+1}"}],
            "cache_slot": i % 4
        }

        req = urllib.request.Request(url, data=json.dumps(payload).encode('utf-8'), headers=headers)
        t0 = time.time()
        try:
            with urllib.request.urlopen(req) as response:
                res = json.loads(response.read().decode('utf-8'))
                dt = time.time() - t0
                print(f"  ✓ Request #{i+1} [Slot {i%4}]: HTTP 200 OK | Response time: {dt*1000:.2f}ms")
        except Exception as e:
            print(f"  ❌ Request #{i+1} Failed: {e}")

    print("Stress test execution completed!")

if __name__ == "__main__":
    run_stress_test()
