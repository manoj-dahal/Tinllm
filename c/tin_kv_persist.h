#ifndef TIN_KV_PERSIST_H
#define TIN_KV_PERSIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tin_kv.h"

static inline bool tin_kv_save_file(const char *filepath, const tin_kv_slot_t *slot) {
    FILE *f = fopen(filepath, "wb");
    if (!f) return false;

    fwrite(&slot->slot_id, sizeof(int), 1, f);
    fwrite(&slot->max_seq_len, sizeof(int), 1, f);
    fwrite(&slot->current_len, sizeof(int), 1, f);
    fwrite(&slot->kv_dim, sizeof(int), 1, f);

    if (slot->current_len > 0) {
        fwrite(slot->kv_data, sizeof(float), slot->current_len * slot->kv_dim, f);
    }

    fclose(f);
    return true;
}

static inline bool tin_kv_load_file(const char *filepath, tin_kv_slot_t *slot) {
    FILE *f = fopen(filepath, "rb");
    if (!f) return false;

    if (fread(&slot->slot_id, sizeof(int), 1, f) != 1 ||
        fread(&slot->max_seq_len, sizeof(int), 1, f) != 1 ||
        fread(&slot->current_len, sizeof(int), 1, f) != 1 ||
        fread(&slot->kv_dim, sizeof(int), 1, f) != 1) {
        fclose(f);
        return false;
    }

    if (slot->current_len > 0 && slot->kv_data) {
        size_t read_floats = fread(slot->kv_data, sizeof(float), slot->current_len * slot->kv_dim, f);
        (void)read_floats;
    }

    fclose(f);
    return true;
}

#endif /* TIN_KV_PERSIST_H */
