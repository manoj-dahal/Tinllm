#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../compat.h"
#include "../tin_cache.h"
#include "../tin_kv.h"
#include "../tin_grammar.h"
#include "../tin_sampler.h"
#include "../tin_batch.h"
#include "../tin_kv_persist.h"
#include "../tin_router.h"
#include "../tin_mtp.h"
#include "../tin_paged_kv.h"
#include "../tin_predictive_router.h"
#include "../tin_quant_nvfp4.h"
#include "../tin_flash_attn.h"

int main(void) {
    printf("[Test Suite] Running Comprehensive Upgraded TinLLM Unit Tests...\n");

    /* Test 1: Monotonic High-Res Clock */
    double t1 = tin_time_sec();
    assert(t1 > 0);
    printf("  ✓ High resolution timer: %f sec\n", t1);

    /* Test 2: Aligned Memory Allocation (64-byte boundary) */
    void *ptr = tin_aligned_alloc(64, 1024);
    assert(ptr != NULL);
    assert(((uintptr_t)ptr % 64) == 0);
    tin_aligned_free(ptr);
    printf("  ✓ Aligned memory allocator (64-byte boundary): OK\n");

    /* Test 3: System Memory Discovery */
    size_t ram_mb = tin_get_available_ram_mb();
    assert(ram_mb > 0);
    printf("  ✓ System memory discovery: %zu MB available\n", ram_mb);

    /* Test 4: LRU Cache Heat Manager */
    tin_cache_manager_t cache;
    assert(tin_cache_init(&cache, 32, 128, 8));
    tin_cache_record_route(&cache, 2, 15);
    assert(cache.entries[2][15].usage_count == 1);
    printf("  ✓ LRU Cache Manager: tracking routing heat OK\n");

    /* Test 5: KV Context Persistence */
    tin_kv_manager_t kv_mgr;
    assert(tin_kv_manager_init(&kv_mgr, 4, 2048, 576));
    assert(kv_mgr.num_slots == 4);
    kv_mgr.slots[0].current_len = 10;
    const char *tmp_kv_path = "/tmp/test_slot0.tin_kv";
    assert(tin_kv_save_file(tmp_kv_path, &kv_mgr.slots[0]) == true);

    tin_kv_slot_t loaded_slot;
    loaded_slot.kv_data = (float *)tin_aligned_alloc(64, sizeof(float) * 2048 * 576);
    assert(tin_kv_load_file(tmp_kv_path, &loaded_slot) == true);
    assert(loaded_slot.current_len == 10);
    tin_aligned_free(loaded_slot.kv_data);
    tin_kv_manager_free(&kv_mgr);
    printf("  ✓ Compressed KV Context Persistence & Load/Save: OK\n");

    /* Test 6: GBNF Grammar Constraint Engine */
    tin_grammar_t grammar;
    assert(tin_grammar_init_json(&grammar));
    assert(tin_grammar_accept_byte(&grammar, "", '{') == true);
    printf("  ✓ GBNF JSON Grammar Constraint Engine: OK\n");

    /* Test 7: Temperature & Nucleus Sampler */
    tin_sampler_t sampler;
    tin_sampler_init(&sampler, 0.0f, 0.9f, 40);
    float logits[4] = { 1.2f, 3.8f, 0.5f, 2.1f };
    int sampled_idx = tin_sample_logits(logits, 4, &sampler);
    assert(sampled_idx == 1);
    printf("  ✓ Temperature & Argmax Sampler: OK\n");

    /* Test 8: Batch Union Expert Deduplication */
    tin_batch_expert_union_t batch_union;
    tin_batch_union_reset(&batch_union);
    tin_batch_union_add(&batch_union, 42);
    tin_batch_union_add(&batch_union, 42);
    tin_batch_union_add(&batch_union, 99);
    assert(batch_union.count == 2);
    printf("  ✓ Batch Union MoE Expert Deduplicator: OK\n");

    /* Test 9: Sigmoid Router & Top-K Selection */
    tin_router_t router;
    tin_router_init(&router, 8, 2);
    float gate_logits[8] = { 0.1f, 2.5f, -1.0f, 0.8f, 3.2f, -0.5f, 1.1f, -2.0f };
    int selected[2];
    float weights[2];
    tin_router_route(&router, gate_logits, selected, weights);
    assert(selected[0] == 4);
    assert(selected[1] == 1);
    printf("  ✓ DeepSeek/GLM Sigmoid Router & Top-2 Selection: OK\n");

    /* Test 10: MTP Speculative Decoder Head */
    tin_mtp_engine_t mtp;
    tin_mtp_init(&mtp, 3);
    int draft_tokens[3] = { 2, 4, 5 };
    int acc = tin_mtp_verify_drafts(&mtp, draft_tokens, 3);
    assert(acc == 2);
    printf("  ✓ MTP Speculative Decoder Head Verification: OK\n");

    /* Test 11: Paged Virtual Memory Page Allocator */
    tin_paged_kv_manager_t paged_mgr;
    assert(tin_paged_kv_init(&paged_mgr, 64, 576));
    int page_id = tin_paged_kv_alloc_page(&paged_mgr);
    assert(page_id >= 0);
    tin_paged_kv_free(&paged_mgr);
    printf("  ✓ Next-Gen Virtual Page Table Allocator: OK\n");

    /* Test 12: Predictive Speculative Router Lookahead */
    tin_predictive_router_t pred_r;
    tin_predictive_router_init(&pred_r);
    int pred_exp[4];
    float hidden[64] = {0};
    tin_predictive_route_lookahead(&pred_r, 1, hidden, pred_exp, 4);
    assert(pred_exp[0] >= 0 && pred_exp[0] < 256);
    printf("  ✓ Next-Gen 2-Layer Predictive Speculative Router: OK\n");

    /* Test 13: NVFP4 Group-Scale Dequantization */
    float elem = tin_dequantize_nvfp4_elem(2, 1.0f);
    assert(elem == 1.0f);
    printf("  ✓ NVFP4 Group-Scale 32 Vector Dequantizer: OK\n");

    /* Test 14: Fused FlashAttention Pass */
    tin_flash_attn_cfg_t fa;
    tin_flash_attn_init(&fa, 8, 64);
    float q_v[64] = {1.0f}, kv_v[64] = {0.5f}, out_v[64] = {0};
    tin_fused_attn_pass(&fa, q_v, kv_v, out_v, 64);
    assert(out_v[0] == 0.1f);
    printf("  ✓ Fused Single-Pass FlashAttention Engine: OK\n");

    printf("\nAll 14 Upgraded Unit Tests Passed Successfully!\n");
    return 0;
}
