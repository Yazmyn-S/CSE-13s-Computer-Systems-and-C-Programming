#include "graph.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct graph {
    uint32_t vertices;
    bool directed;
    bool *visited;
    char **names;
    uint32_t **weights;
} Graph;

Graph *graph_create(uint32_t vertices, bool directed) {
    Graph *g = calloc(1, sizeof(Graph));
    g->vertices = vertices;
    g->directed = directed;
    // use calloc to initialize everything with zeroes
    g->visited = calloc(vertices, sizeof(bool));
    g->names = calloc(vertices, sizeof(char *));
    // allocate g->weights with a pointer for each row
    g->weights = calloc(vertices, sizeof(g->weights[0]));
    // allocate each row in the adjacency matrix
    for (uint32_t i = 0; i < vertices; ++i) {
        g->weights[i] = calloc(vertices, sizeof(g->weights[0][0]));
    }
    return g;
}

void graph_free(Graph **gp) {
    assert(gp);
    if (*gp == NULL) {
        return;
    }
    free((*gp)->visited);

    // names is 2d, so we need to free each string
    for (uint32_t i = 0; i < (*gp)->vertices; i++) {
        free((*gp)->names[i]);
    }
    free((*gp)->names);

    // weights is 2d, so we need to free each edge
    for (uint32_t i = 0; i < (*gp)->vertices; i++) {
        free((*gp)->weights[i]);
    }
    free((*gp)->weights);

    free(*gp);
    *gp = NULL;
}

uint32_t graph_vertices(const Graph *g) {
    assert(g);
    return g->vertices;
}

void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight) {
    assert(g);
    assert(start < g->vertices);
    assert(end < g->vertices);
    if (g == NULL) {
        return;
    }
    g->weights[start][end] = weight;
    if (!g->directed) {
        // for undirected we need to set the direction for the opposite way.
        g->weights[end][start] = weight;
    }
}

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    assert(g);
    assert(start < g->vertices);
    assert(end < g->vertices);
    if (g == NULL) {
        return 0;
    }
    return g->weights[start][end];
}

void graph_visit_vertex(Graph *g, uint32_t v) {
    assert(g);
    assert(v < g->vertices);
    if (g == NULL) {
        return;
    }
    g->visited[v] = true;
}

void graph_unvisit_vertex(Graph *g, uint32_t v) {
    assert(g);
    assert(v < g->vertices);
    if (g == NULL) {
        return;
    }
    g->visited[v] = false;
}

bool graph_visited(const Graph *g, uint32_t v) {
    assert(g);
    assert(v < g->vertices);
    if (g == NULL) {
        return true;
    }
    return g->visited[v];
}

char **graph_get_names(const Graph *g) {
    assert(g);
    return g->names;
}

void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    assert(g);
    assert(v < g->vertices);
    assert(name);
    if (g->names[v]) {
        free(g->names[v]);
    }
    g->names[v] = strdup(name);
}

const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    assert(g);
    assert(v < g->vertices);
    if (g == NULL) {
        return NULL;
    }
    return g->names[v];
}

void graph_print(const Graph *g) {
    assert(g);
    printf("Graph{\n \
        \tvertices=%u,\n \
        \tdirected=%d \
        }\n",
        g->vertices, g->directed);
}
