CC=gcc
CFLAGS=-Wall -g -std=c99
TARGETS=increasing search

all: $(TARGETS)

increasing: increasing.c redirect.c redirect.h
	$(CC) $(CFLAGS) -o $@ $^

search: run-search.h run-search.c search.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o *~ $(TARGETS) a.out