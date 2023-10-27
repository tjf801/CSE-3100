CC := gcc
CFLAGS := -g -Wall -std=c99
TARGETS := pipe-sort

all: $(TARGETS)

pipe-sort: main.c pipe-sort.c pipe-sort.h
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o $(TARGETS)
