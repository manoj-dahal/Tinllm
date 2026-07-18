#ifndef TIN_PAGED_KV_H
#define TIN_PAGED_KV_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "compat.h"

#define PAGE_BLOCK_SIZE 16   // 16 tokens per KV block page
#define MAX_PHYSICAL_PAGES 4096

typedef struct {
    int page_id;
    int ref_count;
    float *kv_data;           // Block size x KV dimension
} tin_kv_page_t;

typedef struct {
    int page_table[512];      // Map virtual block index -> physical page_id
    int total_tokens;
    int num_allocated_pages;
} tin_paged_sequence_t;

typedef struct {
    tin_kv_page_t physical_pool[MAX_PHYSICAL_PAGES];
    int free_pages[MAX_PHYSICAL_PAGES];
    int free_count;
    int kv_dim;
} tin_paged_kv_manager_t;

static inline bool tin_paged_kv_init(tin_paged_kv_manager_t *mgr, int total_pages, int kv_dim) {
    if (total_pages > MAX_PHYSICAL_PAGES) total_pages = MAX_PHYSICAL_PAGES;
    mgr->kv_dim = kv_dim;
    mgr->free_count = total_pages;

    for (int i = 0; i < total_pages; i++) {
        mgr->physical_pool[i].page_id = i;
        mgr->physical_pool[i].ref_count = 0;
        mgr->physical_pool[i].kv_data = (float *)tin_aligned_alloc(64, sizeof(float) * PAGE_BLOCK_SIZE * kv_dim);
        mgr->free_pages[i] = i;
    }
    return true;
}

static inline int tin_paged_kv_alloc_page(tin_paged_kv_manager_t *mgr) {
    if (mgr->free_count <= 0) return -1;
    int page_id = mgr->free_pages[--mgr->free_count];
    mgr->physical_pool[page_id].ref_count = 1;
    return page_id;
}

static inline void tin_paged_kv_free(tin_paged_kv_manager_t *mgr) {
    for (int i = 0; i < MAX_PHYSICAL_PAGES; i++) {
        if (mgr->physical_pool[i].kv_data) {
            tin_aligned_free(mgr->physical_pool[i].kv_data);
        }
    }
}

#endif /* TIN_PAGED_KV_H */
