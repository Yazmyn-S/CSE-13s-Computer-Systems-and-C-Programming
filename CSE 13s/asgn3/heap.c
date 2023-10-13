
#include "heap.h"

uint32_t max_child(Stats *stats, uint32_t *arr, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;

    if (right <= last && cmp(stats, arr[right - 1], arr[left - 1]) == 1) {
        return right;
    }
    return left;
}

void fix_heap(Stats *stats, uint32_t *arr, uint32_t first, uint32_t last) {
    uint32_t mother = first;
    uint32_t great = max_child(stats, arr, first, last);

    while (mother <= last / 2) {
        if (cmp(stats, arr[mother - 1], arr[great - 1]) == -1) {
            swap(stats, &arr[mother - 1], &arr[great - 1]);
            mother = great;
            great = max_child(stats, arr, mother, last);
        } else {
            break;
        }
    }
}

void build_heap(Stats *stats, uint32_t *arr, uint32_t first, uint32_t last) {
    for (uint32_t f = last / 2; f > first - 1; f--) {
        fix_heap(stats, arr, f, last);
    }
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t first = 1;
    uint32_t last = n;
    build_heap(stats, A, first, last);
    for (uint32_t l = last; l > first; l--) {
        swap(stats, &A[l - 1], &A[first - 1]);
        fix_heap(stats, A, first, l - 1);
    }
}
