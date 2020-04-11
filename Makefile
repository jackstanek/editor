OBJS=main.o tty.o common.o
EXECUTABLE=edit

CFLAGS=-g -Wall -Werror

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) Makefile
	$(CC) -o $(EXECUTABLE) $(OBJS)

clean:
	rm -f $(OBJS) $(EXECUTABLE)
