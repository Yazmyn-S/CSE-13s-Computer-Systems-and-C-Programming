CC=clang
debug:   CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic -g
release: CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic
LDFLAGS=-lm

EXECBIN=colorb
OBJS=\
	io.o\
	bmp.o\
	colorb.c\

.PHONY: clean all

all: debug iotest

debug: $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) -o $(EXECBIN)

release: $(EXECBIN)

$(EXECBIN): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $<

iotest: $(OBJS) iotest.c
	$(CC) io.o iotest.c -o iotest

clean:
	rm -rf *.out *.o $(EXECBIN) iotest *.dSYM/ *colorb.bmp

format:
	clang-format -i --style=file *.[ch]

