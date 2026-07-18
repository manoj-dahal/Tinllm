#ifndef TIN_CACHE_H
#define TIN_CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define MAX_LAYERS 80
#define MAX_EXPERTS_PER_LAYER 256

typedef struct {
    int layer_id;
    int expert_id;
    uint32_t usage_count;
    bool is_pinned;
} tin_expert_entry_t;

typedef struct {
    int total_layers;
    int experts_per_layer;
    int cache_slots_per_layer;
    tin_expert_entry_t entries[MAX_LAYERS][MAX_EXPERTS_PER_LAYER];
} tin_cache_manager_t;

static inline bool tin_cache_init(tin_cache_manager_t *cache, int layers, int experts_per_layer, int slots) {
    cache->total_layers = layers;
    cache->experts_per_layer = experts_per_layer;
    cache->cache_slots_per_layer = slots;

    for (int l = 0; l < layers; l++) {
        for (int e = 0; e < experts_per_layer; e++) {
            cache->entries[l][e].layer_id = l;
            cache->entries[l][e].expert_id = e;
            cache->entries[l][e].usage_count = 0;
            cache->entries[l][e].is_pinned = false;
        }
    }
    return true;
}

static inline void tin_cache_record_route(tin_cache_manager_t *cache, int layer, int expert) {
    if (layer < cache->total_layers && expert < cache->experts_per_layer) {
        cache->entries[layer][expert].usage_count++;
    }
}

#endif /* TIN_CACHE_H */
