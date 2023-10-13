CC=clang
debug:   CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic -g
release: CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic
LDFLAGS=-lm

EXECBIN=tsp
OBJS=\
	stack.o\
	graph.o\
	path.o\
	tsp.c\

.PHONY: all debug release clean

all: debug

debug: $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) -o $(EXECBIN)

release: $(EXECBIN)

$(EXECBIN): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $<

test: $(OBJS)
	$(CC) stack.o stack_test.c -o stack.out && ./stack.out

clean:
	rm -rf *.out *.o tsp *.dSYM/

format:
	clang-format -i --style=file *.[ch]
