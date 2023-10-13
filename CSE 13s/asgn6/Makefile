CC=clang
debug:   CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic -g
release: CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic
LDFLAGS=-lm

EXECBIN=huff
OBJS=\
	bitwriter.o\
	node.o\
	pq.o\
	io-x86_64.a\
	$(EXECBIN).c\

.PHONY: clean all

all: debug bwtest nodetest pqtest

debug: $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) -o $(EXECBIN)

release: $(EXECBIN)

$(EXECBIN): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@

dehuff: io-x86_64.a dehuff.c
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $<

bwtest: bitwriter.o bwtest.c
	$(CC) bitwriter.o io-x86_64.a bwtest.c -o bwtest

nodetest: node.o nodetest.c
	$(CC) node.o nodetest.c -o nodetest

pqtest: node.o pq.o pqtest.c
	$(CC) node.o pq.o pqtest.c -o pqtest 

test: bwtest nodetest pqtest
	./bwtest && cat bwtest.out && echo
	./nodetest -v
	./pqtest -v

clean:
	rm -rf *.out *.o $(EXECBIN) dehuff bwtest iotest nodetest pqtest *.dSYM/

format:
	clang-format -i --style=file *.[ch]

