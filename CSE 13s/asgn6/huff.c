
#include "bitwriter.h"
#include "io.h"
#include "node.h"
#include "pq.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS   "i:o:h"
#define HIST_SIZE 256

typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} Code;

void print_help(void) {
    printf("Usage:\n \
    \t-i Sets the file to read from (input file). Requires a filename argument.\n \
    \t-o Sets the file to write to (output file). Requires a filename arugment.\n \
    \t-h Prints the usage of this program.\n");
}

uint32_t fill_histogram(Buffer *buff, double *histogram) {
    assert(buff);
    assert(histogram);

    // Empty the histogram values
    for (uint16_t i = 0; i < HIST_SIZE; i++) {
        histogram[i] = 0.0;
    }

    // Read byte-by-byte and increment the byte count for that byte
    bool finished = false;
    uint32_t filesize = 0;
    while (!finished) {
        // Read each byte in the buffer
        uint8_t byte = 0;
        finished = !read_uint8(buff, &byte);
        if (!finished) {
            ++histogram[byte];
            ++filesize;
        }
    }

    ++histogram[0x00];
    ++histogram[0xff];
    return filesize;
}

Node *create_tree(double *histogram, uint16_t *num_leaves) {
    assert(histogram);
    assert(num_leaves);

    // Step 1 fill PQ
    PriorityQueue *pq = pq_create();

    // Iter through histogram
    for (uint16_t i = 0; i < HIST_SIZE; i++) {
        if (histogram[i] > 0) {
            Node *n = node_create(i, histogram[i]);
            enqueue(pq, n);
        }
    }

    // Huff Algo requires to pop until we get 1 item (or less)
    // Collapses entries until there is 1 left
    while (!pq_is_empty(pq) && !pq_size_is_1(pq)) {
        Node *left;
        Node *right;
        dequeue(pq, &left);
        dequeue(pq, &right);

        // Check if left is a leaf node
        if (left != NULL && left->left == NULL && left->right == NULL) {
            *num_leaves = *num_leaves + 1;
        }
        // Check if right is a leaf node
        if (right != NULL && right->left == NULL && right->right == NULL) {
            *num_leaves = *num_leaves + 1;
        }
        Node *n = node_create(0, left->weight + right->weight);
        n->left = left;
        n->right = right;
        enqueue(pq, n);
    }

    // Step 3: Return the last item
    Node *result = NULL;

    assert(dequeue(pq, &result));
    // Step 4: Delete the Priortiy Queue
    pq_free(&pq);
    return result;
}

void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {
    if (code_table == NULL || node == NULL) {
        return;
    }

    // if node is not a leaf
    if (node->left != NULL && node->right != NULL) {
        fill_code_table(code_table, node->left, code, code_length + 1);
        code |= 1 << code_length;
        fill_code_table(code_table, node->right, code, code_length + 1);
    } else {
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_length = code_length;
    }
}

void huff_write_tree(BitWriter *outbuf, Node *tree) {
    if (outbuf == NULL || tree == NULL) {
        return;
    }

    // Internal Node
    if (tree->left != NULL && tree->right != NULL) {
        huff_write_tree(outbuf, tree->left);
        huff_write_tree(outbuf, tree->right);
        bit_write_bit(outbuf, 0);
    } else {
        // Leaf Node
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, tree->symbol); //Problem: change brt_write_bit to uint8
    }
}

void huff_compress_file(BitWriter *out, Buffer *in, uint32_t filesize, uint16_t num_leaves,
    Node *tree, Code *code_table) {
    if (out == NULL || in == NULL || tree == NULL || code_table == NULL) {
        return;
    }

    // Write 'H'
    bit_write_uint8(out, 'H');

    // Write 'C'
    bit_write_uint8(out, 'C');

    // Write filesize
    bit_write_uint32(out, filesize);

    // Write num_leaves
    bit_write_uint16(out, num_leaves);

    // Recursively write tree
    huff_write_tree(out, tree);

    bool reading = true;
    while (reading) {
        uint8_t byte = 0;
        reading = read_uint8(in, &byte);
        if (!reading) {
            break;
        }
        uint64_t code = code_table[byte].code;
        uint8_t code_length = code_table[byte].code_length;
        for (uint8_t i = 0; i < code_length; i++) {
            bit_write_bit(out, code & 1);
            code >>= 1;
        }
    }
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
        fprintf(stderr, "-o is a required field. Please use -h to see the Usage page.\n");
        return 1;
    }

    // Create Buffers
    Buffer *in = read_open(inFilename);
    if (in == NULL) {
        fprintf(stderr, "failed to create read buffer on input file.\n");
        return 1;
    }

    BitWriter *out = bit_write_open(outFilename);
    if (out == NULL) {
        read_close(&in); // close our allocated buffer for read
        fprintf(stderr, "failed to create write buffer on output file\n");
        return 1;
    }

    // Create Histogram
    double *histogram = calloc(HIST_SIZE, sizeof(double));
    uint32_t filesize = fill_histogram(in, histogram);
    uint16_t num_leaves = 0;

    // Create Tree
    Node *tree = create_tree(histogram, &num_leaves);

    // Create code_table
    Code *code_table = calloc(HIST_SIZE, sizeof(Code));
    fill_code_table(code_table, tree, 0, 0);

    // Compress file
    read_close(&in);
    in = read_open(inFilename);
    if (in == NULL) {
        fprintf(stderr, "failed to create read buffer on input file.\n");
        return 1;
    }
    huff_compress_file(out, in, filesize, num_leaves, tree, code_table);

    // Free Memory to avoid leaks
    read_close(&in);
    bit_write_close(&out);
    free(histogram);
    node_free(&tree);
    free(code_table);
}
