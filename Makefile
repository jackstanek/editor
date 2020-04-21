EXECUTABLE=iv

SRCDIR=src
OBJDIR=build
OBJS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c))

CFLAGS=-g -Wall -Werror
LDFLAGS=-lncurses

$(OBJDIR)/$(EXECUTABLE): $(OBJDIR) $(OBJS)
	$(CC) $(LDFLAGS) -o $(OBJDIR)/$(EXECUTABLE) $(OBJS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$(@F)

.PHONY: clean run
clean:
	$(RM) -r $(OBJDIR)

run: $(OBJDIR)/$(EXECUTABLE)
	$(OBJDIR)/$(EXECUTABLE)
