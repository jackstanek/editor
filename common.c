#include <stdint.h>
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

/* Write value as as string to output.
 * Returns how many digits were converted. */
int fast_itoa(uint8_t value, char* output) {
    uint8_t count = 0;
    while (value > 0) {
	output[count] = '0' + value % 10;
	value /= 10;
	count++;
    }

    /* Reverse the string. */
    for (int i = 0; i < count / 2; i++) {
	char tmp;
	tmp = output[i];
	output[i] = output[count - i - 1];
	output[count - i - 1] = tmp;
    }
    return count;
}
