#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "common.h"
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

    /* Allow writing to stdout */
    struct gbuf active_buffer;
    if (argc == 2) {
	int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
	    fail("open");
	}

	struct stat file_info;
	int rv = fstat(fd, &file_info);
	if (rv < 0) {
	    fail("fstat");
	}

	char* file_contents = calloc(file_info.st_size, sizeof(char));
	if (!file_contents) {
	    fail("calloc");
	}

	if (read(fd, file_contents, file_info.st_size) != file_info.st_size) {
	    fail("read");
	}

	gbuf_init_with_content(&active_buffer, file_info.st_size,
			       file_contents);
	free(file_contents);
    } else {
	gbuf_init(&active_buffer);
    }

    /* Draw tildes on the side */
    for (int i = 0; i < TERM_HEIGHT; i++) {
	write(STDOUT_FILENO, "~", 1);
	move_cursor(STDOUT_FILENO, CUR_DOWN, 1);
	move_cursor(STDOUT_FILENO, CUR_LEFT, 1);
    }
    cursor_top_left(STDOUT_FILENO);

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
