#include "shell.h"

#include "gaps.h"

void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    for (uint32_t g = 0; g < GAPS; g++) {
        uint32_t gap = gaps[g];
        for (uint32_t i = gap; i < length; i++) {
            uint32_t j = i;
            uint32_t temp = move(stats, arr[i]);
            while (j >= gap && cmp(stats, temp, arr[j - gap]) == -1) {
                arr[j] = move(stats, arr[j - gap]);
                j -= gap;
            }
            arr[j] = move(stats, temp);
        }
    }
}
