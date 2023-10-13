#include "path.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct graph {
    uint32_t vertices;
    bool directed;
    bool *visited;
    char **names;
    uint32_t **weights;
} Graph;

typedef struct path {
    uint32_t total_weight;
    Stack *vertices;
} Path;

Path *path_create(uint32_t capacity) {
    Path *p = (Path *) malloc(sizeof(Path));
    p->total_weight = 0;
    p->vertices = stack_create(capacity);
    return p;
}

void path_free(Path **pp) {
    if (*pp == NULL) {
        return;
    }
    stack_free(&(*pp)->vertices);
    free(*pp);
    *pp = NULL;
}

uint32_t path_vertices(const Path *p) {
    assert(p);
    return stack_size(p->vertices);
}

uint32_t path_distance(const Path *p) {
    assert(p);
    return p->total_weight;
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    assert(p);
    assert(g);
    if (stack_empty(p->vertices)) {
        stack_push(p->vertices, val);
        p->total_weight = 0;
        return;
    }
    //get top of stack (stack_peek) push new thing to stack, add distance between stops
    uint32_t src;
    stack_peek(p->vertices, &src);
    stack_push(p->vertices, val);
    p->total_weight += graph_get_weight(g, src, val);
}

uint32_t path_remove(Path *p, const Graph *g) {
    assert(p);
    assert(g);
    uint32_t src;
    uint32_t val;
    if (stack_size(p->vertices) == 1) {
        p->total_weight = 0;
        stack_pop(p->vertices, &src);
        return src;
    }
    stack_pop(p->vertices, &src);
    stack_peek(p->vertices, &val);
    p->total_weight -= graph_get_weight(g, val, src);
    return src;
}

void path_copy(Path *dst, const Path *src) {
    assert(dst);
    assert(src);
    dst->total_weight = src->total_weight;
    stack_copy(dst->vertices, src->vertices);
}

void path_print(const Path *p, FILE *outfile, const Graph *g) {
    assert(p);
    assert(g);
    assert(outfile);
    fprintf(outfile, "Alissa starts at:\n");
    stack_print(p->vertices, outfile, g->names);
    fprintf(outfile, "Total Distance: %d\n", p->total_weight);
}
