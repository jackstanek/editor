OBJS=common.o gapbuffer.o main.o tty.o ui.o
EXECUTABLE=iv

CFLAGS=-g -Wall -Werror

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) Makefile
	$(CC) -o $(EXECUTABLE) $(OBJS)

TEST_GBUF_OBJS=test_gbuf.o gapbuffer.o common.o tty.o
test_gbuf: $(TEST_GBUF_OBJS)
	$(CC) -o test_gbuf $(TEST_GBUF_OBJS)
	./test_gbuf

clean:
	rm -f *.o $(EXECUTABLE)
