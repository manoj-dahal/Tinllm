#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "compat.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./iobench <file_path> [size_mb] [iterations] [threads] [direct 0|1]\n");
        return 1;
    }

    const char *filepath = argv[1];
    int size_mb = (argc > 2) ? atoi(argv[2]) : 19;
    int iters = (argc > 3) ? atoi(argv[3]) : 32;
    int direct = (argc > 5) ? atoi(argv[5]) : 0;

    size_t bytes_to_read = (size_t)size_mb * 1024 * 1024;
    void *buf = tin_aligned_alloc(4096, bytes_to_read);
    if (!buf) {
        fprintf(stderr, "Failed to allocate %d MB aligned buffer\n", size_mb);
        return 1;
    }

    FILE *f = fopen(filepath, "rb");
    if (!f) {
        fprintf(stderr, "Cannot open file: %s\n", filepath);
        tin_aligned_free(buf);
        return 1;
    }

    printf("[TinLLM iobench] Reading %s (%d MB, %d iters, direct=%d)...\n", filepath, size_mb, iters, direct);
    double t0 = tin_time_sec();

    for (int i = 0; i < iters; i++) {
        fseek(f, 0, SEEK_SET);
        size_t read_bytes = fread(buf, 1, bytes_to_read, f);
        (void)read_bytes;
    }

    double elapsed = tin_time_sec() - t0;
    double total_gb = ((double)bytes_to_read * iters) / (1024.0 * 1024.0 * 1024.0);
    double speed_gb_s = total_gb / elapsed;

    printf("Done in %.3f sec | Total read: %.2f GB | Speed: %.2f GB/s\n", elapsed, total_gb, speed_gb_s);

    fclose(f);
    tin_aligned_free(buf);
    return 0;
}
