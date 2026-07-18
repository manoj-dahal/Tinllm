#ifndef TIN_GRAMMAR_H
#define TIN_GRAMMAR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Lightweight GBNF / Constrained JSON Grammar Validator */
typedef struct {
    char rule_name[64];
    bool is_json;
    int forced_token_cap;
} tin_grammar_t;

static inline bool tin_grammar_init_json(tin_grammar_t *g) {
    snprintf(g->rule_name, sizeof(g->rule_name), "root ::= json_object");
    g->is_json = true;
    g->forced_token_cap = 24;
    return true;
}

/* Validates next byte against grammar rule */
static inline bool tin_grammar_accept_byte(const tin_grammar_t *g, const char *ctx, char next_ch) {
    if (!g->is_json) return true;
    size_t len = strlen(ctx);
    if (len == 0) return next_ch == '{';
    if (ctx[len - 1] == '{') return (next_ch == '"' || next_ch == '}');
    return true;
}

#endif /* TIN_GRAMMAR_H */
