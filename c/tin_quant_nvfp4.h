#ifndef TIN_QUANT_NVFP4_H
#define TIN_QUANT_NVFP4_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define GROUP_SIZE_32 32

typedef struct {
    float group_scales[8];  // 256 / 32 = 8 group scales per row
    uint8_t packed_bytes[128]; // 256 4-bit nibbles packed into 128 bytes
} tin_nvfp4_row_t;

static inline float tin_dequantize_nvfp4_elem(uint8_t nibble, float group_scale) {
    static const float nvfp4_lut[16] = {
        0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 3.0f, 4.0f, 6.0f,
        -0.0f, -0.5f, -1.0f, -1.5f, -2.0f, -3.0f, -4.0f, -6.0f
    };
    return nvfp4_lut[nibble & 0x0F] * group_scale;
}

#endif /* TIN_QUANT_NVFP4_H */
