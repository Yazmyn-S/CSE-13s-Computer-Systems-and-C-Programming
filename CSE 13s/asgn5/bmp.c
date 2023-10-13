#include "bmp.h"

#include <assert.h> // assert
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h> // calloc, etc.

typedef struct color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;

typedef struct bmp {
    uint32_t height;
    uint32_t width;
    Color palette[MAX_COLORS];
    uint8_t **a;
} BMP;

BMP *bmp_create(Buffer *buf, bool verbose) {
    if (verbose) {
        verbose = false;
    }

    BMP *bmp = calloc(1, sizeof(BMP));
    if (bmp == NULL) {
        return bmp;
    }

    bmp->height = 0;
    bmp->width = 0;
    uint8_t one_byte;
    uint16_t two_bytes;
    uint32_t four_bytes;

    bool success = false;

    // Read type_1 and assert == 'B'
    success = read_uint8(buf, &one_byte);
    if (!success || one_byte != 'B') {
        bmp_free(&bmp);
        return NULL;
    }

    // Read type_2 and assert == 'M'
    success = read_uint8(buf, &one_byte);
    if (!success || one_byte != 'M') {
        bmp_free(&bmp);
        return NULL;
    }

    // Reads next 12 bytes (4, 2, 2, 4) and does nothing
    // if the reads are successful
    for (int i = 0; i < 3; i++) {
        success = read_uint32(buf, &four_bytes);
        if (!success) {
            bmp_free(&bmp);
            return NULL;
        }
    }

    // Read bitmap_header_size and verify it equals 40
    success = read_uint32(buf, &four_bytes);
    if (!success || four_bytes != 40) {
        bmp_free(&bmp);
        return NULL;
    }

    // Read width
    success = read_uint32(buf, &bmp->width);
    if (!success) {
        bmp_free(&bmp);
        return NULL;
    }
    assert(bmp->width > 0);

    // Read height
    success = read_uint32(buf, &bmp->height);
    if (!success) {
        bmp_free(&bmp);
        return NULL;
    }
    assert(bmp->height > 0);

    // Skip two bytes
    success = read_uint16(buf, &two_bytes);
    if (!success) {
        bmp_free(&bmp);
        return NULL;
    }

    // read bits_per_pixel and verify == 8
    success = read_uint16(buf, &two_bytes);
    if (!success || two_bytes != 8) {
        bmp_free(&bmp);
        return NULL;
    }

    // read in compression and verify == 0
    success = read_uint32(buf, &four_bytes);
    if (!success || four_bytes != 0) {
        bmp_free(&bmp);
        return NULL;
    }

    // Reads next 12 bytes (4, 4, 4) and does nothing
    // if the reads are successful
    for (int i = 0; i < 3; i++) {
        success = read_uint32(buf, &four_bytes);
        if (!success) {
            bmp_free(&bmp);
            return NULL;
        }
    }

    // read num_colors
    uint32_t num_colors = 0;
    success = read_uint32(buf, &num_colors);
    if (!success) {
        bmp_free(&bmp);
        return NULL;
    }

    if (num_colors == 0) {
        num_colors = 1 << 8;
    }

    // skip four bytes
    success = read_uint32(buf, &four_bytes);
    if (!success) {
        bmp_free(&bmp);
        return NULL;
    }

    // read in the colors
    for (uint32_t i = 0; i < num_colors; i++) {
        if (!read_uint8(buf, &bmp->palette[i].blue)) {
            bmp_free(&bmp);
            return NULL;
        }
        if (!read_uint8(buf, &bmp->palette[i].green)) {
            bmp_free(&bmp);
            return NULL;
        }
        if (!read_uint8(buf, &bmp->palette[i].red)) {
            bmp_free(&bmp);
            return NULL;
        }
        // skip the last one
        if (!read_uint8(buf, &one_byte)) {
            bmp_free(&bmp);
            return NULL;
        }
    }

    // Each row must have a multiple of 4 pixels.  Round up to next multiple of 4.
    uint32_t rounded_width = (bmp->width + 3) & ~3;

    // Allocate pixel array
    bmp->a = calloc(rounded_width, sizeof(bmp->a[0]));
    for (uint32_t i = 0; i < rounded_width; i++) {
        bmp->a[i] = calloc(bmp->height, sizeof(bmp->a[i][0]));
    }

    // read pixels
    for (uint32_t y = 0; y < bmp->height; y++) {
        for (uint32_t x = 0; x < rounded_width; x++) {
            if (!read_uint8(buf, &bmp->a[x][y])) {
                bmp_free(&bmp);
                return NULL;
            }
        }
    }

    return bmp;
}

void bmp_free(BMP **bmp) {
    if (*bmp == NULL) {
        return;
    }

    uint32_t rounded_width = ((*bmp)->width + 3) & ~3;
    for (uint32_t i = 0; i < rounded_width; i++) {
        free((*bmp)->a[i]);
    }
    free((*bmp)->a);
    free(*bmp);
    *bmp = NULL;
}

void bmp_write(const BMP *bmp, Buffer *buf) {
    if (bmp == NULL || buf == NULL) {
        return;
    }

    uint32_t rounded_width = (bmp->width + 3) & ~3;
    uint32_t image_size = bmp->height * rounded_width;
    uint32_t file_header_size = 14;
    uint32_t bitmap_header_size = 40;
    uint32_t num_colors = 256;
    uint32_t palette_size = 4 * num_colors;
    uint32_t bitmap_offset = file_header_size + bitmap_header_size + palette_size;
    uint32_t file_size = bitmap_offset + image_size;

    // Write 'B'
    write_uint8(buf, 'B');

    // Write 'M'
    write_uint8(buf, 'M');

    // Write file_size
    write_uint32(buf, file_size);

    // Write the next four bytes of zeros (2, 2)
    write_uint32(buf, 0);

    // bitmap_offset
    write_uint32(buf, bitmap_offset);

    // bitmap_header_size
    write_uint32(buf, bitmap_header_size);

    // Width
    write_uint32(buf, bmp->width);

    // Height
    write_uint32(buf, bmp->height);

    // Write 1
    write_uint16(buf, 1);

    // Write 8
    write_uint16(buf, 8);

    // Write 0
    write_uint32(buf, 0);

    // Write image_size
    write_uint32(buf, image_size);

    // Write 2835
    write_uint32(buf, 2835);

    // Write 2835
    write_uint32(buf, 2835);

    // Write num_colors
    write_uint32(buf, num_colors);

    // Write num_colors
    write_uint32(buf, num_colors);

    for (uint32_t i = 0; i < num_colors; i++) {
        write_uint8(buf, bmp->palette[i].blue);
        write_uint8(buf, bmp->palette[i].green);
        write_uint8(buf, bmp->palette[i].red);
        write_uint8(buf, 0);
    }

    for (uint32_t y = 0; y < bmp->height; y++) {
        for (uint32_t x = 0; x < bmp->width; x++) {
            write_uint8(buf, bmp->a[x][y]);
        }
        // Fill the rest of the row with 0s (because we rounded).
        for (uint32_t x = bmp->width; x < rounded_width; x++) {
            write_uint8(buf, 0);
        }
    }
}

int constrain(int x, int a, int b) {
    return x < a ? a : x > b ? b : x;
}

void bmp_reduce_palette(BMP *bmp) {
    for (int i = 0; i < MAX_COLORS; ++i) {
        int r = bmp->palette[i].red;
        int g = bmp->palette[i].green;
        int b = bmp->palette[i].blue;
        int new_r, new_g, new_b;
        double SQLE = 0.00999 * r + 0.0664739 * g + 0.7317 * b;
        double SELQ = 0.153384 * r + 0.316624 * g + 0.057134 * b;
        if (SQLE < SELQ) {
            // use 575-nm equations
            new_r = 0.426331 * r + 0.875102 * g + 0.0801271 * b + 0.5;
            new_g = 0.281100 * r + 0.571195 * g + -0.0392627 * b + 0.5;
            new_b = -0.0177052 * r + 0.0270084 * g + 1.00247 * b + 0.5;
        } else {
            // use 475-nm equations
            new_r = 0.758100 * r + 1.45387 * g + -1.48060 * b + 0.5;
            new_g = 0.118532 * r + 0.287595 * g + 0.725501 * b + 0.5;
            new_b = -0.00746579 * r + 0.0448711 * g + 0.954303 * b + 0.5;
        }
        new_r = constrain(new_r, 0, UINT8_MAX);
        new_g = constrain(new_g, 0, UINT8_MAX);
        new_b = constrain(new_b, 0, UINT8_MAX);
        bmp->palette[i].red = new_r;
        bmp->palette[i].green = new_g;
        bmp->palette[i].blue = new_b;
    }
}
