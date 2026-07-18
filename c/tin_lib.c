#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "compat.h"
#include "tin_sampler.h"

#if defined(_WIN32)
  #define TIN_API __declspec(dllexport)
#else
  #define TIN_API __attribute__((visibility("default")))
#endif

TIN_API size_t tin_get_ram_mb(void) {
    return tin_get_available_ram_mb();
}

TIN_API float tin_vector_dot(const float *a, const float *b, int len) {
    float sum = 0.0f;
    for (int i = 0; i < len; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

TIN_API int tin_sample_next(const float *logits, int vocab_size, float temp) {
    tin_sampler_t s;
    tin_sampler_init(&s, temp, 0.9f, 40);
    return tin_sample_logits(logits, vocab_size, &s);
}
