#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gapbuffer.h"

const uint64_t INIT_GAP_SIZE = 1 << 8;

/* Update line and column numbers */
void gbuf_update_position(struct gbuf* gbuf, int64_t dist) {
    int di = dist < 0 ? -1 : 1;
    int32_t abs_dist = dist < 0 ? -dist : dist;

    for (uint64_t i = gbuf->gap_begin; i < abs_dist; i += di) {
	if (dist > 0) {
	    if (gbuf->contents[i] == '\n') {
		gbuf->lineno++;
		gbuf->colno = 0;
	    } else {
		gbuf->colno++;
	    }
	} else {
	    if (gbuf->contents[i] == '\n') {
		gbuf->lineno--;

		int cols = 0;
		while (i - cols > 0 && gbuf->contents[i - cols] != '\n') {
		    cols--;
		}
		gbuf->colno = cols;
	    } else {
		gbuf->colno--;
	    }
	}
    }
}

/**
 * Construct a completely empty gap buffer.
 */
int gbuf_init(struct gbuf* gbuf) {
    gbuf->size = INIT_GAP_SIZE;

    gbuf->gap_begin = 0;
    gbuf->gap_end = INIT_GAP_SIZE;

    gbuf->lineno = 0;
    gbuf->colno = 0;

    gbuf->contents = calloc(INIT_GAP_SIZE, sizeof(char));
    if (!gbuf->contents) {
	perror("gbuf_init (empty)");
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

    gbuf->lineno = 0;
    gbuf->colno = 0;

    gbuf->contents = calloc(gbuf->size, sizeof(char));
    if (!gbuf->contents) {
	perror("gbuf_init (content)");
	return 1;
    }

    memcpy(gbuf->contents + gbuf->gap_end, init_contents, size * sizeof(char));
    return 0;
}

void gbuf_destroy(struct gbuf* gbuf) {
    free(gbuf->contents);
}

void gbuf_expand(struct gbuf* gbuf) {
    uint64_t newsize = gbuf->size * 2;
    gbuf->contents = reallocarray(gbuf->contents, newsize, sizeof(char));
    if (!gbuf->contents) {
	perror("gbuf_expand");
    }

    memmove(gbuf->contents + (newsize - gbuf->gap_end),
	    gbuf->contents + gbuf->gap_end,
	    (gbuf->size - gbuf->gap_end) * sizeof(char));
    gbuf->gap_end = newsize - gbuf->gap_end;

    gbuf->size = newsize;
}

void gbuf_insert_char(struct gbuf* gbuf, char c) {
    /* Resize if we ran out of space. */
    if (gbuf->gap_begin >= gbuf->gap_end - 2) {
	gbuf_expand(gbuf);
    }

    if (c == '\r') {
	memcpy(gbuf->contents + gbuf->gap_begin,
	       "\r\n", 2);
	gbuf->gap_begin += 2;
    } else {
	gbuf->contents[gbuf->gap_begin++] = c;
    }

    gbuf_update_position(gbuf, 1);
}

void gbuf_delete_char(struct gbuf* gbuf) {
    if (gbuf->gap_begin > 0) {
	int8_t dist = gbuf->contents[gbuf->gap_begin - 1] == '\n' ? 2 : 1;
	gbuf->gap_begin -= dist;
	gbuf_update_position(gbuf, -dist);
    }
}

uint64_t gbuf_content_size(struct gbuf* gbuf) {
    return gbuf->size - (gbuf->gap_end - gbuf->gap_begin);
}

int64_t gbuf_move_cursor(struct gbuf* gbuf, int64_t dist) {
    if (!dist) {
	/* Moving zero characters is a noop. */
	return 0;
    }

    /* Clamp the move distance. */
    if (gbuf->gap_begin + dist < 0) {
	dist = -gbuf->gap_begin;
    } else if (gbuf->gap_end + dist >= gbuf->size) {
	dist = gbuf->size - gbuf->gap_end;
    }

    int new_begin = gbuf->gap_begin + dist,
	new_end   = gbuf->gap_end   + dist;
    uint32_t abs_dist = dist < 0 ? -dist : dist;

    if (dist > 0) {
	memmove(gbuf->contents + gbuf->gap_begin,
		gbuf->contents + new_end,
		abs_dist * sizeof(char));
    } else {
	memmove(gbuf->contents + gbuf->gap_end,
		gbuf->contents + new_begin,
		abs_dist * sizeof(char));
    }

    gbuf_update_position(gbuf, dist);

    gbuf->gap_begin = new_begin;
    gbuf->gap_end   = new_end;

    return dist;
}
