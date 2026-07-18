#ifndef TIN_BATCH_H
#define TIN_BATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BATCH_SIZE 32
#define MAX_UNIQUE_EXPERTS 256

typedef struct {
    int active_experts[MAX_UNIQUE_EXPERTS];
    int count;
} tin_batch_expert_union_t;

static inline void tin_batch_union_reset(tin_batch_expert_union_t *b) {
    b->count = 0;
    memset(b->active_experts, 0, sizeof(b->active_experts));
}

static inline void tin_batch_union_add(tin_batch_expert_union_t *b, int expert_id) {
    for (int i = 0; i < b->count; i++) {
        if (b->active_experts[i] == expert_id) return; // Deduplicated
    }
    if (b->count < MAX_UNIQUE_EXPERTS) {
        b->active_experts[b->count++] = expert_id;
    }
}

#endif /* TIN_BATCH_H */
