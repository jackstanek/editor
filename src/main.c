#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include <ncurses.h>

#include "gapbuffer.h"

int main(int argc, char* argv[]) {
    struct gbuf active_buffer;
    if (argc == 2) {
	int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
	    perror("open");
	}

	struct stat file_info;
	int rv = fstat(fd, &file_info);
	if (rv < 0) {
	    perror("fstat");
	}

	char* file_contents = calloc(file_info.st_size, sizeof(char));
	if (!file_contents) {
	    perror("calloc");
	}

	if (read(fd, file_contents, file_info.st_size) != file_info.st_size) {
	    perror("read");
	}

	gbuf_init_with_content(&active_buffer, file_info.st_size,
			       file_contents);
	free(file_contents);
    } else {
	gbuf_init(&active_buffer);
    }

    /* initialize ncurses */
    initscr();

    int c;
    while (1) {
	c = getch();
	if (c == 'q') {
	    break;
	}
	refresh();
    }
    endwin();

    return 0;
}
