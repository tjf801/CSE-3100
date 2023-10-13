TARGETS=run2
SRCS=$(patsubst %,%.c,$(TARGETS))
CC=gcc
CFLAGS= -Wall -g -std=c99

all : $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean: 
	@rm -f $(TARGETS) *.o a.out

