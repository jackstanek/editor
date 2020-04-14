#ifndef GAPBUFFER_H
#define GAPBUFFER_H

#include <stdint.h>

struct gbuf {
    uint64_t size;
    uint64_t gap_begin;
    uint64_t gap_end;
    char* contents;
};

int gbuf_init(struct gbuf* gbuf);
int gbuf_init_with_content(struct gbuf* gbuf, uint64_t size, char* init_contents);
void gbuf_expand(struct gbuf* gbuf);
void gbuf_insert_char(struct gbuf* gbuf, char c);
uint64_t gbuf_content_size(struct gbuf* gbuf);
void gbuf_move_cursor(struct gbuf* gbuf, int64_t dist);

#endif
