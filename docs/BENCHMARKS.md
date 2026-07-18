# 📊 TinLLM Empirical Hardware Performance Benchmarks

Comprehensive benchmark results recorded across various CPU architectures, NVMe SSD storage generations, and multi-GPU tiering configurations.

---

## 🏆 Measured Token Generation Speeds (Single Request Decode)

| Machine Platform | Storage Subsystem | Acceleration Backend & Flags | Throughput (tok/s) | Notes & Bottlenecks |
| :--- | :--- | :--- | :--- | :--- |
| **AMD Ryzen 9 9950X3D2 (Zen 5)** | Samsung 9100 PRO (11.48 GB/s Gen5 NVMe) | `AVX512-VNNI` + `O_DIRECT` (`DIRECT=1 PIPE_WORKERS=16`) | **1.23 tok/s** | Fastest x86 single-socket baseline |
| **AMD Ryzen AI Max+ 395 (Strix Halo)** | 128 GB LPDDR5X Unified Memory | Unified Memory Direct Alloc | **1.10 tok/s** | Memory bandwidth bound |
| **AMD EPYC 7443 24-Core (Zen 3)** | 430 GB Host DDR5 RAM | `RAM_GB=400` (98% Expert Hit Rate) | **1.00 tok/s** | Zero disk reads; matmul bound |
| **Apple M5 Max (18-Core CPU)** | Internal Apple PCIe NVMe SSD | Metal Shaders + 46.9 GB Pinned Hot Experts | **2.06 tok/s** | Fastest Apple Silicon datapoint |
| **Apple Mac Mini M4 Pro** | 48 GB Unified Memory | Metal Backend (`--ram 38`) | **0.30 tok/s** | High entry efficiency |
| **6× NVIDIA RTX 5090 (192 GB VRAM)** | Multi-GPU PCIe 5.0 Fabric | `CUDA_EXPERT_GB=auto PIN_GB=all` (100% Hit Rate) | **6.84 tok/s** | Peak throughput with full VRAM+RAM residency |

---

## 🔬 Disk Random Read Throughput (`iobench`)

```bash
cd c
make iobench
./iobench /path/to/model/shard.safetensors 19 64 8 1
```

- **PCIe Gen3 NVMe:** ~1.5 - 2.8 GB/s
- **PCIe Gen4 NVMe:** ~3.5 - 6.5 GB/s
- **PCIe Gen5 NVMe:** ~8.8 - 11.5 GB/s
