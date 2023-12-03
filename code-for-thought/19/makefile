TARGETS=server client_smart
SRCS=$(patsubst %,%.c,$(TARGETS))
CC=gcc
CFLAGS= -Wall -g -pthread -O3

all : $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean: 
	@rm -f $(TARGETS) *.o a.out 

run: all
	./server & ./client_smart 127.0.0.1 < input.txt > result.txt
	@rm -f $(TARGETS) *.o a.out 

