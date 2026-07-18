#ifndef TIN_ASYNC_IO_H
#define TIN_ASYNC_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "compat.h"

#define MAX_IO_WORKERS 16

typedef struct {
    int worker_id;
    pthread_t thread;
    bool is_active;
} tin_io_worker_t;

typedef struct {
    int num_workers;
    tin_io_worker_t workers[MAX_IO_WORKERS];
    bool pipe_enabled;
    bool pilot_prefetch_enabled;
} tin_async_io_pool_t;

static inline bool tin_async_io_init(tin_async_io_pool_t *pool, int num_workers) {
    if (num_workers > MAX_IO_WORKERS) num_workers = MAX_IO_WORKERS;
    pool->num_workers = num_workers;
    pool->pipe_enabled = true;
    pool->pilot_prefetch_enabled = true;

    for (int i = 0; i < num_workers; i++) {
        pool->workers[i].worker_id = i;
        pool->workers[i].is_active = true;
    }
    return true;
}

#endif /* TIN_ASYNC_IO_H */
