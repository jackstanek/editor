#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "tty.h"

const char* ED2 = "\x1b[2J";
const char* HVHOME = "\x1b[f";

void reset_termios() {
  revert_terminal_mode(STDOUT_FILENO);
}

void fail(char* reason) {
  perror(reason);

  reset_termios();
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  /* Set terminal in raw mode */
  enable_raw_mode(STDOUT_FILENO);
  atexit(reset_termios);

  write(STDOUT_FILENO, ED2, 4);
  write(STDOUT_FILENO, HVHOME, 3);

  char c;
  while (read(STDOUT_FILENO, &c, 1) == 1 && c != '\x11') {
    write(STDOUT_FILENO, HVHOME, 3);
    write(STDOUT_FILENO, &c, 1);
  }

  return 0;
}
