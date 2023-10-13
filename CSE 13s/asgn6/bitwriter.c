
#include "bitwriter.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct BitWriter BitWriter;

struct BitWriter {
    Buffer *buf;
    uint8_t byte;
    uint8_t bit;
};

BitWriter *bit_write_open(const char *filename) {
    // Allocate a new BitWriter
    BitWriter *bw = calloc(1, sizeof(BitWriter));
    assert(bw != NULL);

    // Create the underlying buff
    Buffer *buf = write_open(filename);
    assert(buf != NULL);

    bw->buf = buf;
    bw->byte = 0;
    bw->bit = 0;
    return bw;
}

void bit_write_close(BitWriter **pbw) {
    if (*pbw == NULL) {
        return;
    }

    if ((*pbw)->bit) {
        write_uint8((*pbw)->buf, (*pbw)->byte);
    }

    // Write to disk
    write_close(&(*pbw)->buf);

    free(*pbw);
    *pbw = NULL;
}

void bit_write_bit(BitWriter *bw, uint8_t bit) {
    if (bw == NULL) {
        return;
    }

    // Write byte if bit_position > 7
    if (bw->bit > 7) {
        write_uint8(bw->buf, bw->byte);
        bw->byte = 0;
        bw->bit = 0;
    }
    if (bit & 1) {
        bw->byte |= (bit & 1) << bw->bit;
    }
    bw->bit++;
}

void bit_write_uint8(BitWriter *bw, uint8_t byte) {
    if (bw == NULL) {
        return;
    }

    // byte: 0110 01|10  & 0000 0001
    // i: 1
    for (uint8_t i = 0; i < 8; i++) {
        bit_write_bit(bw, (byte >> i) & 1);
    }
}

void bit_write_uint16(BitWriter *bw, uint16_t x) {
    if (bw == NULL) {
        return;
    }

    for (uint16_t i = 0; i < 16; i++) {
        bit_write_bit(bw, (x >> i) & 1);
    }
}
void bit_write_uint32(BitWriter *bw, uint32_t x) {
    if (bw == NULL) {
        return;
    }

    for (uint32_t i = 0; i < 32; i++) {
        bit_write_bit(bw, (x >> i) & 1);
    }
}
