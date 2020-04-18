#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "common.h"
#include "tty.h"

static struct termios orig;
int TERM_WIDTH, TERM_HEIGHT;

/* Erase display */
const char* ED2 = "\x1b[2J";

/* Cursor top left */
const char* HVHOME = "\x1b[f";

/**
 * Update the terminal size variables. Should be run at program
 * initialization and whenever the terminal is resized (i.e. when
 * SIGWINCH is received.)
 */
void update_term_size(int fd) {
    struct winsize ws;
    int rv = ioctl(fd, TIOCGWINSZ, &ws);
    if (rv) {
	fail("ioctl");
    }

    TERM_WIDTH = ws.ws_row;
    TERM_HEIGHT = ws.ws_col;
}

/**
 * Put the TTY in "raw mode". Essentially does the same thing as
 * cfmakeraw() in some libc implementations.
 */
void enable_raw_mode(int fd) {
    struct termios new_termios;
    tcgetattr(fd, &orig);
    tcgetattr(fd, &new_termios);

    /* see cfmakeraw(3) */
    new_termios.c_lflag &= ~ECHO;

    new_termios.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
			     | INLCR | IGNCR | ICRNL | IXON);
    new_termios.c_oflag &= ~OPOST;
    new_termios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    new_termios.c_cflag &= ~(CSIZE | PARENB);
    new_termios.c_cflag |= CS8;

    tcsetattr(fd, TCSANOW, &new_termios);
}

/**
 * After calling enable_raw_mode, this will revert the TTY to its
 * state previous to the call.
 */
void revert_term_mode(int fd) {
    tcsetattr(fd, TCSANOW, &orig);
}

/**
 * Erase all screen contents.
 */
void clear_term(int fd) {
    write(fd, ED2, 4);
}

/**
 * Position the cursor in the top left corner
 */
void cursor_top_left(int fd) {
    write(fd, HVHOME, 3);
}

/**
 * Move the cursor in the specified direction for distance positions.
 */
void move_cursor(int fd, int direction, uint8_t distance) {
    int num_chars_to_write = 3; /* ESC, [, and direction. */
    char cmdbuf[6] = "\x1b[0000";
    int digits_len = fast_itoa(distance, &cmdbuf[2]);
    num_chars_to_write += digits_len;
    cmdbuf[digits_len + 2] = 'A' + direction; /* set the direction, i.e. the last
						 letter of the command. */
    write(fd, cmdbuf, num_chars_to_write);
}
