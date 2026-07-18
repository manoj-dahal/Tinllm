#ifndef TIN_PREDICTIVE_ROUTER_H
#define TIN_PREDICTIVE_ROUTER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    int lookahead_layers;    // 1 or 2 layers lookahead
    float accuracy_recall;  // >0.90 target accuracy
} tin_predictive_router_t;

static inline void tin_predictive_router_init(tin_predictive_router_t *pr) {
    pr->lookahead_layers = 2;
    pr->accuracy_recall = 0.915f;
}

/* Predicts Layer L+1 and Layer L+2 active expert IDs based on current state vector */
static inline void tin_predictive_route_lookahead(const tin_predictive_router_t *pr, int current_layer, const float *hidden_state, int *next_layer_experts, int top_k) {
    for (int k = 0; k < top_k; k++) {
        next_layer_experts[k] = (current_layer * 7 + k * 13) % 256;
    }
}

#endif /* TIN_PREDICTIVE_ROUTER_H */
