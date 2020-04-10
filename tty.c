#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "tty.h"

static struct termios orig;

void enable_raw_mode(int fd) {
  struct termios new_termios;
  tcgetattr(fd, &orig);
  tcgetattr(fd, &new_termios);

  /* see cfmakeraw(3) if on Linux or BSD. */
  new_termios.c_lflag &= ~ECHO;

  new_termios.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                           | INLCR | IGNCR | ICRNL | IXON);
  new_termios.c_oflag &= ~OPOST;
  new_termios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  new_termios.c_cflag &= ~(CSIZE | PARENB);
  new_termios.c_cflag |= CS8;

  tcsetattr(fd, TCSANOW, &new_termios);
}

void revert_terminal_mode(int fd) {
  tcsetattr(fd, TCSANOW, &orig);
}
