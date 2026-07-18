#ifndef TIN_KV_H
#define TIN_KV_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "compat.h"

typedef struct {
    int slot_id;
    int max_seq_len;
    int current_len;
    int kv_dim; // Compressed MLA dimension (e.g. 576 floats)
    float *kv_data;
} tin_kv_slot_t;

typedef struct {
    int num_slots;
    tin_kv_slot_t *slots;
} tin_kv_manager_t;

static inline bool tin_kv_manager_init(tin_kv_manager_t *m, int num_slots, int max_seq_len, int kv_dim) {
    m->num_slots = num_slots;
    m->slots = (tin_kv_slot_t *)malloc(sizeof(tin_kv_slot_t) * num_slots);
    if (!m->slots) return false;

    for (int i = 0; i < num_slots; i++) {
        m->slots[i].slot_id = i;
        m->slots[i].max_seq_len = max_seq_len;
        m->slots[i].current_len = 0;
        m->slots[i].kv_dim = kv_dim;
        m->slots[i].kv_data = (float *)tin_aligned_alloc(64, sizeof(float) * max_seq_len * kv_dim);
    }
    return true;
}

static inline void tin_kv_manager_free(tin_kv_manager_t *m) {
    if (!m) return;
    for (int i = 0; i < m->num_slots; i++) {
        if (m->slots[i].kv_data) {
            tin_aligned_free(m->slots[i].kv_data);
        }
    }
    free(m->slots);
}

#endif /* TIN_KV_H */
