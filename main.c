#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "gapbuffer.h"
#include "tty.h"

void reset_termios() {
    revert_term_mode(STDOUT_FILENO);
}

int main(int argc, char* argv[]) {
    /* Set terminal in raw mode */
    update_term_size(STDOUT_FILENO);
    enable_raw_mode(STDOUT_FILENO);
    atexit(reset_termios);

    clear_term(STDOUT_FILENO);
    cursor_top_left(STDOUT_FILENO);

    /* Draw tildes on the side */
    for (int i = 0; i < TERM_HEIGHT; i++) {
	write(STDOUT_FILENO, "~", 1);
	move_cursor(STDOUT_FILENO, CUR_DOWN, 1);
	move_cursor(STDOUT_FILENO, CUR_LEFT, 1);
    }
    cursor_top_left(STDOUT_FILENO);

    /* Allow writing to stdout */
    struct gbuf active_buffer;
    gbuf_init(&active_buffer);

    char keypress;
    while (read(STDOUT_FILENO, &keypress, 1) == 1) {
	/* TODO: We really gotta fix rendering updates. */
	if (keypress == 0x11) { /* ^Q */
	    break;
	} else if (keypress == 0x7f) { /* DEL */
	    gbuf_delete_char(&active_buffer);
	    move_cursor(STDOUT_FILENO, CUR_LEFT, 1);
	    write(STDOUT_FILENO, " ", 1);
	    move_cursor(STDOUT_FILENO, CUR_LEFT, 1);
	} else {
	    cursor_top_left(STDOUT_FILENO);
	    gbuf_insert_char(&active_buffer, keypress);
	    write(STDOUT_FILENO,
		  active_buffer.contents,
		  active_buffer.gap_begin);
	    write(STDOUT_FILENO,
		  active_buffer.contents + active_buffer.gap_end,
		  active_buffer.size - active_buffer.gap_end);
	}
    }

    return 0;
}
