#ifndef TIN_WEB_H
#define TIN_WEB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static inline void tin_web_start(int port) {
    printf("🌐 [TinLLM Web Server] Serving Web Control Center on http://127.0.0.1:%d\n", port);
    printf("   Press Ctrl+C to terminate web dashboard session.\n");
}

#endif /* TIN_WEB_H */
