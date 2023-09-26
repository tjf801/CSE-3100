TARGETS=diffusion monopoly
CC=gcc
CFLAGS=-Wall -g -std=c99

all : $(TARGETS)

$(TARGETS): %: %.c

clean: 
	@rm -f $(TARGETS) a.out *.o

sub:
	@zip $${PWD##*/}.zip $(SRCS) Makefile
