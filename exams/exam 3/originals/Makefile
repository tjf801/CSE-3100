CC=gcc
CFLAGS=-Wall -g -pthread
TARGETS=k-means dice

all: $(TARGETS)

k-means: k-means.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

dice: dice.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o *~ $(TARGETS) a.out