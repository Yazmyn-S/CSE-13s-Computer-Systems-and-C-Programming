#include "stats.h"

#include <stdio.h>

int cmp(Stats *stats, uint32_t x, uint32_t y) {
    stats->compares++;
    if (x < y) {
        return -1;
    }
    if (x > y) {
        return 1;
    }
    return 0;
}

uint32_t move(Stats *stats, uint32_t x) {
    stats->moves++;
    return x;
}

void swap(Stats *stats, uint32_t *x, uint32_t *y) {
    stats->moves += 3;
    uint32_t temp = *x;
    *x = *y;
    *y = temp;
}

void reset(Stats *stats) {
    stats->moves = 0;
    stats->compares = 0;
}

void print_stats(Stats *stats, const char *algorithm_name, uint32_t num_elements) {
    printf("%s, %" PRIu32 " elements, "
           "%" PRIu64 " moves, "
           "%" PRIu64 " compares\n",
        algorithm_name, num_elements, stats->moves, stats->compares);
}
