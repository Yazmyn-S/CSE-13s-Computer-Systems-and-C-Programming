#include "bmp.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS     "i:o:h"
#define VERTEX_NAME 1000

void print_help(void) {
    printf("Usage:\n \
    \t-i Sets the file to read from (input file). Requires a filename argument.\n \
    \t-o Sets the file to write to (output file). Requires a filename arugment.\n \
    \t-d Treats all graphs as directed. By default, all graphs are undirected.\n \
    \t-h Prints the usage of this program.\n");
}

int main(int argc, char **argv) {
    // Flag fields
    int opt;
    char *inFilename = NULL;
    char *outFilename = NULL;

    // Read Flags
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': inFilename = optarg; break;
        case 'o': outFilename = optarg; break;
        case 'h': print_help(); return 0;
        }
    }

    // Check for Required Fields
    if (inFilename == NULL) {
        fprintf(stderr, "-i is a required field. Please use -h to see the Usage page.\n");
        return 1;
    }
    if (outFilename == NULL) {
        fprintf(stderr, "-i is a required field. Please use -h to see the Usage page.\n");
        return 1;
    }

    // Create Buffers
    Buffer *in = read_open(inFilename);
    if (in == NULL) {
        fprintf(stderr, "failed to create read buffer on input file.\n");
        return 1;
    }

    Buffer *out = write_open(outFilename);
    if (out == NULL) {
        read_close(&in); // close our allocated buffer for read
        fprintf(stderr, "failed to create write buffer on output file\n");
        return 1;
    }

    // Create BMP
    BMP *bmp = bmp_create(in, false);
    if (bmp == NULL) {
        read_close(&in);
        write_close(&out);
        fprintf(stderr, "failed to create bmp. Is this a valid bmp file?\n");
        return 1;
    }

    // Apply Transformation
    bmp_reduce_palette(bmp);

    // Write to output file
    bmp_write(bmp, out);

    // Free Memory to avoid leaks
    read_close(&in);
    write_close(&out);
    bmp_free(&bmp);
}
