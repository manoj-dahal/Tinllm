#ifndef TIN_ROUTER_H
#define TIN_ROUTER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    int num_experts;
    int top_k;
    float routed_scaling_factor;
} tin_router_t;

static inline void tin_router_init(tin_router_t *r, int num_experts, int top_k) {
    r->num_experts = num_experts;
    r->top_k = top_k;
    r->routed_scaling_factor = 2.5f;
}

/* Compute sigmoid gating scores and select top-K expert indices */
static inline void tin_router_route(const tin_router_t *r, const float *gate_logits, int *selected_experts, float *expert_weights) {
    float scores[256];
    for (int i = 0; i < r->num_experts; i++) {
        scores[i] = 1.0f / (1.0f + expf(-gate_logits[i]));
    }

    for (int k = 0; k < r->top_k; k++) {
        int max_idx = 0;
        float max_val = -1.0f;
        for (int i = 0; i < r->num_experts; i++) {
            if (scores[i] > max_val) {
                max_val = scores[i];
                max_idx = i;
            }
        }
        selected_experts[k] = max_idx;
        expert_weights[k] = max_val * r->routed_scaling_factor;
        scores[max_idx] = -100.0f;
    }
}

#endif /* TIN_ROUTER_H */
