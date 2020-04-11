#ifndef TTY_H
#define TTY_H

#include <stdint.h>

enum cursor_direction { CUR_UP = 0, CUR_DOWN, CUR_RIGHT, CUR_LEFT};

void enable_raw_mode(int fd);
void revert_term_mode(int fd);
void update_term_size(int fd);

void clear_term(int fd);
void cursor_top_left(int fd);
void move_cursor(int fd, int direction, uint8_t distance);

extern int TERM_WIDTH, TERM_HEIGHT;

#endif
