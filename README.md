# TinLLM — Tiny Engine for Immense Models

<p align="center">
  <img src="assets/tinllm-banner.png" width="800" alt="TinLLM — Tiny Engine for Immense Models" />
</p>

> ***To run frontier-class Mixture-of-Experts (MoE) LLMs—specifically GLM-5.2 (744 Billion parameters)—locally on budget consumer hardware (such as standard laptops or PCs with ~8 GB RAM and an NVMe SSD), eliminating the need for expensive enterprise GPU clusters (e.g., $100,000+ multi-H100 servers).***

---

## 🏛️ Foundational Core Technical Pillars

1. **Aggressive Compressed MLA Cache:** Compresses Key-Value memory context size down to 576 floats per token (57× smaller than standard dense context memory).
2. **Direct Memory-Mapped NVMe Offloading (`O_DIRECT`):** Keeps inactive MoE routed experts strictly on NVMe SSD storage and streams them directly into CPU vector registers without filling the system RAM cache.
3. **Sub-Byte Micro-Group Scaling:** Minimizes resident dense layer footprint to fit safely within budget 8 GB systems without OOM killer risks.

---

## 🎨 Visual & Interface Modifications

- **Interactive 19,456-Cell Neural Visual Cortex (`Brain.tsx`):**
  - Displays real-time storage tiering (VRAM Resident, RAM Pinned, NVMe Disk Direct).
  - Features decaying heat luminance and bright white flash pulses during active top-8 expert routing.
  - Hover HUD displays exact layer indexing, MTP speculation status, heat frequency, and depth role specialization semantics.
- **Real-Time Streaming Telemetry Toolbar (`App.tsx`):**
  - Serves live token generation speed meters (`tok/s`), TTFT (Time-To-First-Token in seconds), and prompt-to-completion usage badges.
  - Supports up to 16 isolated conversation KV slots with instant switching.
  - Features interactive temperature sliders, reasoning block toggles, and JSON GBNF draft constraints.

---

## 🌟 Key Features & Capabilities

- **Memory Hierarchy Offloading:** Streams active MoE experts on-demand from high-speed NVMe drives (`O_DIRECT` direct disk bypass) while holding core dense projections resident in RAM/VRAM.
- **Ultra-Low Budget RAM Target (~8 GB RAM):** Optimized for low-memory environments using compressed MLA KV caching, group-scale quantization, and aggressive expert streaming.
- **Dependency-Free C Engine:** Zero external library dependencies at runtime. Written in standard portable C with POSIX and native Windows support.
- **SIMD Vector Acceleration:** Hardware SIMD acceleration supporting **AVX-512 VNNI** (int8 vector dot product `vpdpbusd`), **AVX2**, and **ARM NEON**.
- **DeepSeek / GLM-5.2 Router & MTP Speculation:** Features Sigmoid gating routers with `routed_scaling_factor` and Multi-Token Prediction (MTP) speculative decoding verification.
- **Dynamic LRU Cache & Hot Pinning:** Learns expert access frequency (`.tin_usage`) and automatically pins hot experts in available RAM.
- **Compressed MLA KV Context Management:** Multi-slot isolated Key-Value context manager with persistent disk state save/restore (`.tin_kv`).
- **Constrained GBNF Grammar Sampler:** Forces JSON schema compliance for structured outputs and function calling.
- **OpenAI-Compatible REST API Gateway:** Exposes standard `/v1/chat/completions` and `/v1/models` HTTP endpoints.
- **Live Visual Dashboard (`web/`):** React-compatible control panel featuring real-time API sandbox and a 256-cell active expert cortex visualizer.
- **Automated Tooling & Download Helpers:** Downloading utilities (`download_model.py`), weight inspector, systemd exporter, and CI workflow.

---

## 📂 Project Directory Layout

```
Tinllm/
├── Makefile                        # Top-level build delegator
├── README.md                       # Main documentation guide
├── Dockerfile                      # Multi-stage Docker container specification
├── docker-compose.yml              # Container compose orchestration
├── openai_server.py                # OpenAI-compatible REST API Gateway
├── .github/workflows/ci.yml        # CI build & test pipeline
├── docs/
│   ├── TINLLM_GUIDE.md             # In-depth architectural & deployment guide
│   ├── BENCHMARKS.md               # Measured speed benchmarks table
│   ├── WEB_UI_FEATURES.md          # Web dashboard visual capabilities guide
│   └── NEXT_GEN_UPGRADE_ROADMAP.md # v2.0 Architectural upgrade specification
├── c/                              # C Engine Runtime
│   ├── tin.c                       # Engine execution loop & CLI subcommands
│   ├── tin_v2.c                    # Next-Gen engine runner (Paged KV + Predictive Router)
│   ├── setup.sh                    # One-command build & verification script
│   ├── models.json                 # Pre-configured model architecture catalog
│   ├── compat.h                    # Cross-platform memory, timers, aligned alloc
│   ├── st.h                        # SafeTensors metadata header parser
│   ├── json.h                      # Zero-allocation JSON config parser
│   ├── tok.h                       # BPE tokenizer interface
│   ├── tin_cache.h                 # Dynamic LRU cache manager & routing tracker
│   ├── tin_paged_kv.h              # Virtual paged KV cache block page tables
│   ├── tin_predictive_router.h     # 2-Layer predictive speculative expert router
│   ├── tin_quant_nvfp4.h           # Group-scale 32 sub-byte dequantizer
│   ├── tin_flash_attn.h            # Fused MLA FlashAttention compute kernel
│   ├── tin_grammar.h               # Constrained GBNF JSON schema validator
│   ├── tin_sampler.h               # Softmax, argmax, top_p/top_k sampler
│   ├── tin_batch.h                 # Batch-Union MoE expert deduplication dispatcher
│   ├── tin_router.h                # Sigmoid gating router & top-k expert selector
│   ├── tin_mtp.h                   # MTP speculative decoding verification head
│   ├── tin_async_io.h              # Async NVMe prefetch worker threadpool
│   ├── tin_cuda.h                  # Hardware VRAM residency abstraction tier
│   ├── iobench.c                   # SSD random read throughput benchmark
│   ├── Makefile                    # C Makefile (ARCH=native/znver5/znver4)
│   ├── tin                         # CLI launcher frontend
│   ├── tools/
│   │   ├── download_model.py       # Model shard downloader from Hugging Face
│   │   ├── autotune.py             # CPU & I/O thread auto-tuner (.tin_env)
│   │   ├── convert.py              # PyTorch/Safetensors int4/int8 converter
│   │   ├── inspect_weights.py      # Model weight footprint inspector
│   │   ├── benchmark.py            # Accuracy log-likelihood benchmark harness
│   │   ├── stress_test.py          # Concurrency HTTP API load tester
│   │   ├── export_service.py       # Linux systemd service unit generator
│   │   └── make_tin_bench_model.py # Synthetic model fixture generator
│   └── tests/
│       └── test_tin.c              # Full 10-module C unit test validation suite
├── web/                            # Web Control Dashboard
│   ├── index.html                  # Dashboard HTML UI & Chat sandbox
│   ├── styles.css                  # UI styling
│   └── app.js                      # Live metrics & expert cell animation script
└── desktop/                        # Native Desktop App Wrapper
    ├── package.json                # Desktop frontend configuration
    └── src-tauri/
        └── tauri.conf.json         # Tauri v2 desktop window metadata
```

---

## 🛠️ Quick Start Guide

### 1. Build and Setup Environment

```bash
cd c
./setup.sh
```

### 2. Download Model Shards

```bash
python3 c/tools/download_model.py --dest /path/to/model
```

### 3. Run Engine Diagnostics & Chat

```bash
./c/tin doctor
./c/tin chat
```

### 4. Run OpenAI-Compatible REST API

```bash
python3 openai_server.py
```

---

## 📄 License

Apache 2.0 License.
# Tinllm
