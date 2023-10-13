#include "io.h"

#include <assert.h> // assert
#include <fcntl.h> // open
#include <stdlib.h> // calloc, etc.
#include <unistd.h> // write

struct buffer {
    // file descriptor from open() or creat()
    int fd;

    // offset into buffer a[]
    //     next valid byte (reading)
    //     next empty location (writing)
    int offset;

    // number of bytes remaining in buffer (reading)
    int num_remaining;

    uint8_t a[BUFFER_SIZE];
};

Buffer *read_open(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }
    Buffer *buf = calloc(1, sizeof(Buffer));
    buf->fd = fd;
    buf->offset = 0;
    buf->num_remaining = 0; // empty buffer
    return buf;
}

void read_close(Buffer **pbuf) {
    if (*pbuf == NULL) {
        return;
    }
    close((*pbuf)->fd);
    free(*pbuf);
    *pbuf = NULL;
}

bool read_uint8(Buffer *buf, uint8_t *x) {
    // Do try if any pointer is invalid
    if (buf == NULL || x == NULL) {
        return false;
    }

    // the buf is empty, we need to fill buf->a[BUFFER_SIZE] or read until EOF
    if (buf->num_remaining == 0) {
        ssize_t rc = read(buf->fd, buf->a, sizeof(buf->a));
        assert(rc >= 0);
        if (rc == 0) {
            // EOF
            return false;
        }
        buf->num_remaining = rc;
        buf->offset = 0;
    }

    // read the byte
    *x = buf->a[buf->offset++];

    // we're at end of valid buffer
    if (buf->offset == buf->num_remaining) {
        buf->num_remaining = 0;
    }

    return true;
}

// f      = 0000 0000 0010 1010
// s      = 1000 0101 0000 0000
// result = 1000 0101 0010 1010

bool read_uint16(Buffer *buf, uint16_t *x) {
    if (buf == NULL || x == NULL) {
        return false;
    }
    uint8_t first, second = 0;
    bool fr, sr = false;

    fr = read_uint8(buf, &first);
    if (fr) {
        // try to read the next byte because we are not EOF
        sr = read_uint8(buf, &second);
    }

    // Put the result into x
    *x = ((second << 8) | first);
    return fr && sr;
}

bool read_uint32(Buffer *buf, uint32_t *x) {
    if (buf == NULL || x == NULL) {
        return false;
    }
    uint16_t first, second = 0;
    bool fr, sr = false;

    fr = read_uint16(buf, &first);
    if (fr) {
        // try to read the next 4 bytes because we are not EOF
        sr = read_uint16(buf, &second);
    }

    // Put the result into x
    *x = ((second << 16) | first);
    return fr && sr;
}

Buffer *write_open(const char *filename) {
    int fd = creat(filename, 0664);
    if (fd < 0) {
        return NULL;
    }
    Buffer *buf = calloc(1, sizeof(Buffer));
    buf->fd = fd;
    buf->offset = 0;
    buf->num_remaining = 0; // unused for writes
    return buf;
}

void write_close(Buffer **pbuf) {
    if (*pbuf == NULL) {
        return;
    }

    // If buffer is not empty, write remaining bytes to disk.
    if ((*pbuf)->offset > 0) {
        uint8_t *start = (*pbuf)->a;
        int num_bytes = (*pbuf)->offset;
        do {
            ssize_t rc = write((*pbuf)->fd, start, num_bytes);
            assert(rc >= 0);
            start += rc;
            num_bytes -= rc;
        } while (num_bytes > 0);
    }

    close((*pbuf)->fd);
    free(*pbuf);
    *pbuf = NULL;
}

void write_uint8(Buffer *buf, uint8_t x) {
    if (buf == NULL) {
        return;
    }

    // write the byte
    buf->a[buf->offset++] = x;

    // if the buffer is now full, write to disk
    if (buf->offset == BUFFER_SIZE) {
        uint8_t *start = buf->a;
        int num_bytes = buf->offset;
        do {
            ssize_t rc = write(buf->fd, start, num_bytes);
            assert(rc >= 0);
            start += rc;
            num_bytes -= rc;
        } while (num_bytes > 0);
        buf->offset = 0;
    }
}

void write_uint16(Buffer *buf, uint16_t x) {
    write_uint8(buf, x);
    write_uint8(buf, x >> 8);
}

void write_uint32(Buffer *buf, uint32_t x) {
    write_uint16(buf, x);
    write_uint16(buf, x >> 16);
}
