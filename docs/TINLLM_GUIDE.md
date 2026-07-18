# TinLLM Deployment & Architecture Guide

> ***To run frontier-class Mixture-of-Experts (MoE) LLMs—specifically GLM-5.2 (744 Billion parameters)—locally on budget consumer hardware (such as standard laptops or PCs with ~8 GB RAM and an NVMe SSD), eliminating the need for expensive enterprise GPU clusters (e.g., $100,000+ multi-H100 servers).***

---

## 🏛️ Foundational Core Technical Pillars

1. **Aggressive Compressed MLA Cache:** Compresses Key-Value memory context size down to 576 floats per token (57× smaller than standard dense context memory).
2. **Direct Memory-Mapped NVMe Offloading (`O_DIRECT`):** Keeps inactive MoE routed experts strictly on NVMe SSD storage and streams them directly into CPU vector registers without filling the system RAM cache.
3. **Sub-Byte Micro-Group Scaling:** Minimizes resident dense layer footprint to fit safely within budget 8 GB systems without OOM killer risks.

---

## Technical Specifications

### Memory Management Strategy
TinLLM divides model memory requirements into two tiers:
1. **Resident Hot Tier (RAM/VRAM):**
   - Base dense layers, embeddings, final normalization, compressed KV-cache, and frequently accessed "hot" experts.
2. **Cold Direct NVMe Streaming Tier:**
   - Inactive MoE routed experts live on disk (~370 GB container for 744B MoE) and are read asynchronously on demand using `O_DIRECT` direct disk streaming.

### Memory Sizing Formula
$$\text{Resident Footprint (GB)} = \text{Dense Params (int4)} + \text{KV Slots} \times (L \times D_{kv} \times 4 \text{ bytes}) + \text{Pinned Expert Cache}$$

---

## Performance Tuning Recipes

### 1. AMD Zen Processors (Zen 4 & Zen 5)
```bash
cd c
make ARCH=znver5                   # Target Zen 5 AVX-512 VNNI SIMD
export OMP_NUM_THREADS=16          # Bind to physical cores
export OMP_PLACES=cores
export OMP_PROC_BIND=close
export DIRECT=1 URING=1 PIPE=1     # Direct I/O streaming
./tin chat
```

### 2. Apple Silicon (M-Series)
```bash
cd c
make ARCH=native                   # Uses ARM NEON intrinsics
./tin chat
```

---

## API & Gateway Usage

Start the OpenAI REST API gateway:
```bash
python3 openai_server.py
```

Send a request via `curl`:
```bash
curl http://127.0.0.1:8080/v1/chat/completions \
  -H "Content-Type: application/json" \
  -d '{
    "model": "tinllm-moe",
    "messages": [{"role": "user", "content": "Explain Mixture-of-Experts offloading"}],
    "temperature": 0.7
  }'
```

---

## Diagnostic Commands

- `./c/tin doctor` — Check SIMD kernels, memory bounds, and NVMe alignment.
- `./c/tin plan` — Export JSON execution resource plan.
- `./c/tin bench` — Run SIMD vector multiplication performance benchmarks.
- `make test` — Execute unit tests for memory alignment, LRU cache, and sampler.
