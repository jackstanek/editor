#include <stdio.h>

#include "gapbuffer.h"

int main(int argc, char* argv[]) {
	struct gbuf gbuf;
	gbuf_init(&gbuf);
	gbuf_insert_char(&gbuf, '2');
	gbuf_move_cursor(&gbuf, -1);
	gbuf_insert_char(&gbuf, '4');
	gbuf_move_cursor(&gbuf, 2);
	gbuf_insert_char(&gbuf, '0');

	printf("%s\n", gbuf.contents);
}
