#!/usr/bin/env python3
"""
TinLLM Native Python Ctypes Binding Library
Provides direct high-speed Python bindings to the C engine shared library (libtin.so).
"""

import ctypes
import os
import sys

def load_tin_lib():
    lib_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    
    if sys.platform.startswith("win"):
        lib_name = "tin.dll"
    elif sys.platform.startswith("darwin"):
        lib_name = "libtin.dylib"
    else:
        lib_name = "libtin.so"

    lib_path = os.path.join(lib_dir, lib_name)
    if not os.path.exists(lib_path):
        raise FileNotFoundError(f"Shared library not found at '{lib_path}'. Run 'make lib' in 'c/' first.")

    lib = ctypes.CDLL(lib_path)
    
    lib.tin_get_ram_mb.restype = ctypes.c_size_t
    lib.tin_get_ram_mb.argtypes = []

    lib.tin_sample_next.restype = ctypes.c_int
    lib.tin_sample_next.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int, ctypes.c_float]

    return lib

class TinEngine:
    def __init__(self):
        self.lib = load_tin_lib()

    def available_ram_mb(self) -> int:
        return self.lib.tin_get_ram_mb()

    def sample_logits(self, logits: list, temp: float = 0.7) -> int:
        c_logits = (ctypes.c_float * len(logits))(*logits)
        return self.lib.tin_sample_next(c_logits, len(logits), temp)

if __name__ == "__main__":
    print("[TinLLM Python Binding] Testing Ctypes wrapper module...")
    try:
        engine = TinEngine()
        print(f"  ✓ System RAM available via C engine: {engine.available_ram_mb()} MB")
        sample_logits = [0.1, 2.8, 0.4, 1.2]
        token = engine.sample_logits(sample_logits, temp=0.0)
        print(f"  ✓ Sampled argmax token: #{token} (Expected #1)")
    except Exception as e:
        print(f"Note: Build libtin shared library first ('make lib' in c/). Details: {e}")
