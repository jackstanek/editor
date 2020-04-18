#include <unistd.h>

#include "common.h"
#include "gapbuffer.h"
#include "tty.h"
#include "ui.h"

int refresh_display(int fd, struct gbuf* gbuf) {
    clear_term(fd);
    cursor_top_left(fd);
    write(fd, gbuf->contents,
	  gbuf->gap_begin);
    write(fd, gbuf->contents + gbuf->gap_end,
	  gbuf->size - gbuf->gap_end);

    /* TODO Improve this please! */
    char pos[8];
    int i = fast_itoa(gbuf->lineno, pos);
    pos[i++] = ',';
    fast_itoa(gbuf->colno,  pos + (i++));
    for (; i < 8; i++) {
	pos[i] = 0;
    }

    write(fd, "\x1b[s", 3);
    write(fd, "\x1b[23;0H", 7);
    write(fd, pos, 8);
    write(fd, "\x1b[u", 3);

    return 0;
}
