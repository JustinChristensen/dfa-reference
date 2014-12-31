PROGRAM = strip_comments
CFLAGS =
LDFLAGS =

all: $(PROGRAM)

$(PROGRAM): main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $+

.PHONY: all
