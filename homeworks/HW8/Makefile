TARGETS=food
CC=gcc
CFLAGS=-Wall -g -pthread -std=gnu99

all : $(TARGETS)

$(TARGETS): %: %.c

clean: 
	@rm -f $(TARGETS) a.out *.o
