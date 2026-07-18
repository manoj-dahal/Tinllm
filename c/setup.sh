#!/usr/bin/env bash
set -e

echo "============================================================"
echo "  🐦 TinLLM Environment Setup & Verification Utility"
echo "============================================================"

# Detect GCC / Clang Compiler
if command -v gcc >/dev/null 2>&1; then
    CC="gcc"
elif command -v clang >/dev/null 2>&1; then
    CC="clang"
else
    echo "❌ Error: No C compiler found (gcc or clang required)."
    exit 1
fi

echo "✓ Compiler found: $CC ($($CC --version | head -n 1))"

# Check OpenMP Support
echo "Checking OpenMP support..."
if $CC -fopenmp -x c - -o /tmp/omp_test 2>/dev/null <<EOF
#include <omp.h>
int main() { return omp_get_num_threads(); }
EOF
then
    echo "✓ OpenMP acceleration supported"
    rm -f /tmp/omp_test
else
    echo "⚠️ Warning: OpenMP non-functional or missing. Compiling without OpenMP."
fi

# Build C Engine
echo -e "\nBuilding TinLLM Engine (ARCH=native)..."
make clean
make ARCH=native

# Run Self-Test Suite
echo -e "\nExecuting unit test validation..."
make test

# Auto-Tune Hardware Parameters
if command -v python3 >/dev/null 2>&1; then
    echo -e "\nExecuting hardware auto-tuner..."
    python3 tools/autotune.py
fi

echo "============================================================"
echo "🎉 TinLLM setup completed successfully!"
echo "Run './tin chat' or 'python3 ../openai_server.py' to begin."
echo "============================================================"
