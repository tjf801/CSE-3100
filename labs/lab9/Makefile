TARGETS=server client
SRCS=$(patsubst %,%.c,$(TARGETS))
CC=gcc
CFLAGS= -Wall -g -pthread -Wextra -std=gnu99

all : $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean: 
	@rm -f $(TARGETS) *.o a.out 

sub: 
	@zip $${PWD##*/}.zip $(SRCS) Makefile
