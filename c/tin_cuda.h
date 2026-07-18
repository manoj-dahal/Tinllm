#ifndef TIN_CUDA_H
#define TIN_CUDA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int device_id;
    size_t total_vram_mb;
    size_t free_vram_mb;
    bool is_available;
} tin_cuda_device_t;

static inline bool tin_cuda_detect(tin_cuda_device_t *dev) {
#if defined(TIN_CUDA)
    dev->device_id = 0;
    dev->total_vram_mb = 24576; // Sample 24GB VRAM
    dev->free_vram_mb = 20000;
    dev->is_available = true;
    return true;
#else
    dev->device_id = -1;
    dev->total_vram_mb = 0;
    dev->free_vram_mb = 0;
    dev->is_available = false;
    return false;
#endif
}

#endif /* TIN_CUDA_H */
