#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

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

    for (int i = 0; i < TERM_HEIGHT; i++) {
	write(STDOUT_FILENO, "~", 1);
	move_cursor(STDOUT_FILENO, CUR_DOWN, 1);
	move_cursor(STDOUT_FILENO, CUR_LEFT, 1);
    }

    return 0;
}
