#ifndef TTY_H
#define TTY_H

void enable_raw_mode(int fd);
void revert_terminal_mode(int fd);

#endif
