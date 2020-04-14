#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "gapbuffer.h"

const uint64_t INIT_GAP_SIZE = 1 << 8;

/**
 * Construct a completely empty gap buffer.
 */
int gbuf_init(struct gbuf* gbuf) {
    gbuf->size = INIT_GAP_SIZE;
    gbuf->gap_begin = 0;
    gbuf->gap_end = INIT_GAP_SIZE;

    gbuf->contents = calloc(INIT_GAP_SIZE, sizeof(char));
    if (!gbuf->contents) {
	fail("gbuf_init (empty)");
    }

    return 0;
}

/**
 * Construct a gap buffer with some initial contents.
 */
int gbuf_init_with_content(struct gbuf* gbuf, uint64_t size,
			   char* init_contents) {
    gbuf->size = size + INIT_GAP_SIZE;
    gbuf->gap_begin = 0;
    gbuf->gap_end = INIT_GAP_SIZE;

    gbuf->contents = calloc(gbuf->size, sizeof(char));
    if (!gbuf->contents) {
	fail("gbuf_init (content)");
	return 1;
    }

    memcpy(gbuf->contents + gbuf->gap_end, init_contents, size * sizeof(char));
    return 0;
}

void gbuf_expand(struct gbuf* gbuf) {
    uint64_t newsize = gbuf->size * 2;
    gbuf->contents = reallocarray(gbuf->contents, newsize, sizeof(char));
    if (!gbuf->contents) {
	fail("gbuf_expand");
    }

    memmove(gbuf->contents + (newsize - gbuf->gap_end),
	    gbuf->contents + gbuf->gap_end, gbuf->size - gbuf->gap_end);
    gbuf->gap_end = newsize - gbuf->gap_end;

    gbuf->size = newsize;
}

void gbuf_insert_char(struct gbuf* gbuf, char c) {
    /* Resize if we ran out of space. */
    if (gbuf->gap_begin >= gbuf->gap_end - 1) {
	gbuf_expand(gbuf);
    }

    gbuf->contents[gbuf->gap_begin++] = c;
}

uint64_t gbuf_content_size(struct gbuf* gbuf) {
    return gbuf->size - (gbuf->gap_end - gbuf->gap_begin);
}

void gbuf_move_cursor(struct gbuf* gbuf, int64_t dist) {
}
