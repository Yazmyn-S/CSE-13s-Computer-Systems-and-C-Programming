#include "graph.h"
#include "path.h"
#include "stack.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS     "i:o:dh"
#define VERTEX_NAME 1000

void print_help(void) {
    printf("Usage:\n \
    \t-i Sets the file to read from (input file). Requires a filename argument. Default file to read from is stdin.\n \
    \t-o Sets the file to write to (output file). Requires a filename arugment. Default file to write to is stdout.\n \
    \t-d Treats all graphs as directed. By default, all graphs are undirected.\n \
    \t-h Prints the usage of this program.\n");
}

Graph *create_graph(FILE *f, bool directed) {
    assert(f);
    // Read number of vertices
    uint32_t num_vertices;
    if ((fscanf(f, "%u\n", &num_vertices)) != 1) {
        fprintf(stderr, "tsp: error failed to read number of vertices");
        exit(1);
    }

    Graph *g = graph_create(num_vertices, directed);

    // Read the names of the verticies.
    for (uint32_t i = 0; i < num_vertices; i++) {
        char name[VERTEX_NAME];

        //causes the newline problem!!!
        fgets(name, VERTEX_NAME, f); // reads entire line
        for (uint32_t i = 0; i < strlen(name); i++) {
            if (name[i] == '\n') {
                name[i] = '\0';
            }
        }
        graph_add_vertex(g, name, i);
    }

    // Read the number of edges
    uint32_t num_edges;
    if ((fscanf(f, "%u\n", &num_edges)) != 1) {
        fprintf(stderr, "tsp: error failed to read number of edges");
        exit(1);
    }

    // Read edges and load into graph
    for (uint32_t i = 0; i < num_edges; i++) {
        uint32_t start, end, weight;
        if ((fscanf(f, "%u %u %u\n", &start, &end, &weight)) != 3) {
            fprintf(stderr, "tsp: error failed to read edge");
            exit(1);
        }
        graph_add_edge(g, start, end, weight);
    }

    return g;
}

void dfs(uint32_t v, Graph *g, Path *curr, Path **best) {
    assert(g);
    assert(curr);
    assert(best);
    graph_visit_vertex(g, v);
    path_add(curr, v, g);

    // for all of v's neighbors, we need to visit them if we haven't already
    for (uint32_t i = 0; i < graph_vertices(g); i++) {
        if (graph_get_weight(g, v, i) > 0 && !graph_visited(g, i)) {
            dfs(i, g, curr, best);
        }
    }

    // We've traversed this path as far as possible.
    // We should check if the path has all verticies
    // If yes, we should add another stop back Home (vertex 0)
    //   // then compare if curr path is better than best
    //   // if it's better than best, we copy curr into best
    // Lastly, we should pop this vertex from the path
    // Mark this vertex as unvisited

    // Checking to see if path has all vertices and there is a way home.
    if (path_vertices(curr) == graph_vertices(g) && graph_get_weight(g, v, 0) > 0) {
        // Add distance from the last stop to back home.
        // This makes path a Hamilton cycle.
        path_add(curr, 0, g);

        if (*best == NULL) {
            // This is the first completed cycle.
            *best = path_create(path_vertices(curr));
            path_copy(*best, curr);
        } else if (path_distance(curr) < path_distance(*best)) {
            // This path is more optimal than best
            path_copy(*best, curr);
        }

        // Remove adding the last stop home
        path_remove(curr, g);
    }

    path_remove(curr, g);
    graph_unvisit_vertex(g, v);
}

Path *tsp(Graph *g) {
    assert(g);
    Path *best = NULL;

    // To store the way back home too.
    Path *curr = path_create(graph_vertices(g) + 1);

    // Only try to DFS if there is at least one-vertex in the map
    if (graph_vertices(g) > 0) {
        dfs(0, g, curr, &best);
    }

    // Free curr here because it would leak otherwise.
    path_free(&curr);

    return best;
}

int main(int argc, char **argv) {
    // Flag fields
    int opt;
    char *inFilename = NULL;
    char *outFilename = NULL; //this could the problem!!!
    bool directed = false;

    // Read Flags
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': inFilename = optarg; break;
        case 'o': outFilename = optarg; break;
        case 'd': directed = true; break;
        case 'h': print_help(); return 0;
        }
    }

    // Try to open input file
    FILE *infile = stdin;
    if (inFilename != NULL) {
        infile = fopen(inFilename, "r");
        if (infile == NULL) {
            fprintf(stderr, "tsp: failed to open input file: %s\n", inFilename);
            exit(1);
        }
    }

    // Try to open output file
    FILE *outfile = stdout;
    if (outFilename != NULL) {
        outfile = fopen(outFilename, "w");
        if (outfile == NULL) {
            fprintf(stderr, "tsp: failed to open output file: %s\n", outFilename);
            exit(1);
        }
    }

    // TSP and Result
    Graph *g = create_graph(infile, directed);
    Path *p = tsp(g);
    if (p != NULL) {
        path_print(p, outfile, g);
    } else {
        fprintf(outfile, "No path found! Alissa is lost!\n");
    }

    // Close File Handlers
    if (infile != stdin) {
        fclose(infile);
    }
    if (outfile != stdout) {
        fclose(outfile);
    }

    // Free all Paths and Graph
    graph_free(&g);
    path_free(&p);
}
