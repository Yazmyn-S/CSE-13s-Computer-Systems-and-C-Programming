#include "batcher.h"

void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y) {
    if (cmp(stats, A[x], A[y]) == 1) {
        swap(stats, &A[x], &A[y]);
    }
}

uint32_t bit_length(uint32_t n) {
    uint32_t bits = 0;
    while (n) {
        n >>= 1;
        bits++;
    }
    return bits;
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    if (n == 0) {
        return;
    }

    uint64_t t = bit_length(n);
    uint64_t p = 1 << (t - 1);

    while (p) {
        uint64_t q = 1 << (t - 1);
        uint64_t r = 0;
        uint64_t d = p;

        while (d) {
            for (uint64_t i = 0; i < n - d; i++) {
                if ((i & p) == r) {
                    comparator(stats, A, i, i + d);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
