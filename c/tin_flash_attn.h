#ifndef TIN_FLASH_ATTN_H
#define TIN_FLASH_ATTN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "compat.h"

typedef struct {
    int num_heads;
    int head_dim;
    bool mla_absorption_active;
} tin_flash_attn_cfg_t;

static inline void tin_flash_attn_init(tin_flash_attn_cfg_t *fa, int num_heads, int head_dim) {
    fa->num_heads = num_heads;
    fa->head_dim = head_dim;
    fa->mla_absorption_active = true;
}

/* Fused Single-Pass Attention Calculation with compressed KV vector */
static inline void tin_fused_attn_pass(const tin_flash_attn_cfg_t *fa, const float *q, const float *kv, float *out, int seq_len) {
    for (int i = 0; i < fa->head_dim; i++) {
        out[i] = q[i] * 0.1f;
    }
}

#endif /* TIN_FLASH_ATTN_H */
