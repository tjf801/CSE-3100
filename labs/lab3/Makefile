# define more variables so it is easier to make changes
# again, study the new varabiles and rules
CC=gcc
CFLAGS=-g -Wall -std=c99
TARGETS=argvcat squares

all: $(TARGETS)

#Static Pattern Rules
$(TARGETS): % : %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o *~ $(TARGETS)
