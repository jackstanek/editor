#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "tty.h"

inline void fail(char* reason) {
    perror(reason);

    revert_term_mode(STDOUT_FILENO);
    exit(EXIT_FAILURE);
}
