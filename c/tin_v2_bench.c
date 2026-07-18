#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "compat.h"
#include "tin_paged_kv.h"
#include "tin_predictive_router.h"
#include "tin_quant_nvfp4.h"
#include "tin_flash_attn.h"

int main(void) {
    printf("============================================================\n");
    printf("🚀 TinLLM v2.0 Next-Gen Architectural Micro-Benchmark Suite\n");
    printf("============================================================\n");

    /* Benchmark 1: Paged Memory Page Allocation Throughput */
    tin_paged_kv_manager_t mgr;
    tin_paged_kv_init(&mgr, 2048, 576);

    double t0 = tin_time_sec();
    for (int i = 0; i < 1000; i++) {
        int p = tin_paged_kv_alloc_page(&mgr);
        (void)p;
    }
    double dt_paged = tin_time_sec() - t0;
    printf("  ✓ Paged Virtual KV Allocation Rate: %.2f million page allocations/sec\n", 1000.0 / (dt_paged * 1000.0));

    /* Benchmark 2: Predictive Speculative Router Lookahead Speed */
    tin_predictive_router_t pr;
    tin_predictive_router_init(&pr);

    t0 = tin_time_sec();
    int pred_experts[8];
    float hidden[128] = {0};
    for (int i = 0; i < 100000; i++) {
        tin_predictive_route_lookahead(&pr, 1, hidden, pred_experts, 8);
    }
    double dt_router = tin_time_sec() - t0;
    printf("  ✓ Predictive Router 2-Layer Speculation: %.2f million predictions/sec\n", 100000.0 / (dt_router * 1000000.0));

    /* Benchmark 3: NVFP4 Element Dequantization Lookup Speed */
    t0 = tin_time_sec();
    float dummy_val = 0.0f;
    for (int i = 0; i < 1000000; i++) {
        dummy_val += tin_dequantize_nvfp4_elem(i % 16, 1.25f);
    }
    double dt_dequant = tin_time_sec() - t0;
    printf("  ✓ Group-32 Scale NVFP4 Dequantization: %.2f million elements/sec (checksum: %.1f)\n", 1000000.0 / (dt_dequant * 1000000.0), dummy_val);

    /* Benchmark 4: Fused Single-Pass MLA FlashAttention */
    tin_flash_attn_cfg_t fa;
    tin_flash_attn_init(&fa, 64, 128);
    float q[128] = {1.0f}, kv[128] = {0.5f}, out[128] = {0};
    t0 = tin_time_sec();
    for (int i = 0; i < 50000; i++) {
        tin_fused_attn_pass(&fa, q, kv, out, 128);
    }
    double dt_attn = tin_time_sec() - t0;
    printf("  ✓ Fused Multi-Head Latent FlashAttention: %.2f thousand passes/sec\n", 50000.0 / (dt_attn * 1000.0));

    tin_paged_kv_free(&mgr);
    printf("============================================================\n");
    printf("🎉 Next-Gen Benchmark Execution Complete!\n");
    printf("============================================================\n");
    return 0;
}
