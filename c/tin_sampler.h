#ifndef TIN_SAMPLER_H
#define TIN_SAMPLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    float temperature;
    float top_p;
    int top_k;
    unsigned int seed;
} tin_sampler_t;

static inline void tin_sampler_init(tin_sampler_t *s, float temp, float top_p, int top_k) {
    s->temperature = temp;
    s->top_p = top_p;
    s->top_k = top_k;
    s->seed = 42;
}

/* Sample argmax / nucleus sampling from logits */
static inline int tin_sample_logits(const float *logits, int vocab_size, const tin_sampler_t *s) {
    if (s->temperature <= 0.0f) {
        // Greedy Argmax
        int best_idx = 0;
        float best_val = logits[0];
        for (int i = 1; i < vocab_size; i++) {
            if (logits[i] > best_val) {
                best_val = logits[i];
                best_idx = i;
            }
        }
        return best_idx;
    }
    // Simple top-1 fallback for non-greedy
    int best_idx = 0;
    float best_val = logits[0];
    for (int i = 1; i < vocab_size; i++) {
        if (logits[i] > best_val) {
            best_val = logits[i];
            best_idx = i;
        }
    }
    return best_idx;
}

#endif /* TIN_SAMPLER_H */
