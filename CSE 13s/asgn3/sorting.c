#include "batcher.h"
#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS      "ishqbdar:n:p:H"
#define DEFAULT_SIZE 100
#define DEFAULT_SEED 13371453
#define MASK         1073741823 // 0b111111111111111111111111111111

// Sort Flags
int INSERTION_SORT = 0;
int SHELL_SORT = 1;
int HEAP_SORT = 2;
int QUICK_SORT = 3;
int BATCHER_SORT = 4;

void print_array(const uint32_t *arr, uint32_t length, uint32_t elements) {
    for (uint32_t i = 0; i < length && i < elements; i++) {
        if (i != 0 && i % 5 == 0) {
            printf("\n");
        }
        printf("%13" PRIu32, arr[i]);
    }

    if (length && elements) {
        // Trailing newline
        printf("\n");
    }
}

void copy_arr(const uint32_t *arr, uint32_t *cpy, int size) {
    for (int i = 0; i < size; i++) {
        cpy[i] = arr[i];
    }
}

void print_help(void) {
    printf("SYNOPSIS\n"
           "\tA collection of comparison-based sorting algorithms.\n"
           "USAGE\n"
           "\t./sorting [-Hahbsqi] [-n length] [-p elements] [-r seed]\n"
           "OPTIONS\n"
           "-H              Display program help and usage.\n"
           "-a              Enable all sorts.\n"
           "-h              Enable Heap Sort.\n"
           "-b              Enable Batcher Sort.\n"
           "-s              Enable Shell Sort.\n"
           "-q              Enable Quick Sort.\n"
           "-i              Enable Insertion Sort.\n"
           "-n length       The size of the array (default: 100)\n"
           "-r seed         The random seed (default: 13371453)\n"
           "-p elements     The number of elements to display\n");
}

int main(int argc, char **argv) {
    Set sortFlags = set_empty();

    // Read flags
    int debugMode = 0;
    int opt = 0;
    int seed = DEFAULT_SEED;
    int size = DEFAULT_SIZE;
    int elements = DEFAULT_SIZE;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': sortFlags = set_insert(sortFlags, INSERTION_SORT); break;
        case 's': sortFlags = set_insert(sortFlags, SHELL_SORT); break;
        case 'h': sortFlags = set_insert(sortFlags, HEAP_SORT); break;
        case 'q': sortFlags = set_insert(sortFlags, QUICK_SORT); break;
        case 'b': sortFlags = set_insert(sortFlags, BATCHER_SORT); break;
        case 'r': seed = strtoul(optarg, NULL, 0); break;
        case 'n': size = strtoul(optarg, NULL, 0); break;
        case 'p': elements = strtoul(optarg, NULL, 0); break;
        case 'H': print_help(); return 0;
        case 'a': sortFlags = set_universal(); break;
        case 'd': debugMode = 1; break;
        default: print_help(); return 0;
        }
    }

    if (debugMode) {
        printf("Debug Mode\n");
        printf("---------------------------------\n");
        printf("\tsize=%d\n", size);
        printf("\tseed=%d\n", seed);
        printf("\telements=%d\n", elements);
        printf("\tinsertion_sort=%d\n", set_member(sortFlags, INSERTION_SORT));
        printf("\theap_sort=%d\n", set_member(sortFlags, HEAP_SORT));
        printf("\tshell_sort=%d\n", set_member(sortFlags, SHELL_SORT));
        printf("\tquick_sort=%d\n", set_member(sortFlags, QUICK_SORT));
        printf("\tbatcher_sort=%d\n", set_member(sortFlags, BATCHER_SORT));
        printf("---------------------------------\n");
    }

    // Get Random Array
    uint32_t *arr = calloc(size, sizeof(uint32_t));
    if (arr == NULL) {
        printf("failed to allocate enough memory for the array");
        exit(1);
    }
    srandom(seed);
    for (int i = 0; i < size; i++) {
        arr[i] = random() & MASK;
    }
    uint32_t *cpy = calloc(size, sizeof(uint32_t));
    assert(cpy != NULL);

    Stats s;
    reset(&s); // initializes all fields

    // Insertion Sort
    if (set_member(sortFlags, INSERTION_SORT)) {
        copy_arr(arr, cpy, size);
        insertion_sort(&s, cpy, size);
        print_stats(&s, "Insertion Sort", size);
        print_array(cpy, size, elements);
        reset(&s);
    }

    // Heap Sort
    if (set_member(sortFlags, HEAP_SORT)) {
        copy_arr(arr, cpy, size);
        heap_sort(&s, cpy, size);
        print_stats(&s, "Heap Sort", size);
        print_array(cpy, size, elements);
        reset(&s);
    }

    // Shell Sort
    if (set_member(sortFlags, SHELL_SORT)) {
        copy_arr(arr, cpy, size);
        shell_sort(&s, cpy, size);
        print_stats(&s, "Shell Sort", size);
        print_array(cpy, size, elements);
        reset(&s);
    }

    // Quicksort
    if (set_member(sortFlags, QUICK_SORT)) {
        copy_arr(arr, cpy, size);
        quick_sort(&s, cpy, size);
        print_stats(&s, "Quick Sort", size);
        print_array(cpy, size, elements);
        reset(&s);
    }

    // Batcher
    if (set_member(sortFlags, BATCHER_SORT)) {
        copy_arr(arr, cpy, size);
        batcher_sort(&s, cpy, size);
        print_stats(&s, "Batcher Sort", size);
        print_array(cpy, size, elements);
        reset(&s);
    }

    // Free the array
    free(arr);
    free(cpy);
}
