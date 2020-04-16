#include <unistd.h>

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

    return 0;
}
