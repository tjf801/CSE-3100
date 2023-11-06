TARGET1=test-mmul
OBJS1=test-mmul.o matrix.o mmul.o 
SRCS1=$(patsubst %.o,%.c,$(OBJS1))
HEADERS1=matrix.h

TARGET2=printing
SRCS2=$(patsubst %,%.c,$(TARGET2))

TARGETS=$(TARGET1) $(TARGET2)
CC=gcc
CFLAGS= -Wall -pthread -g -std=c99

all : $(TARGETS)

$(TARGET1): $(OBJS1)
	$(CC) $(CFLAGS) -o $@ $^

clean: 
	@rm -f $(OBJS1) $(TARGETS)

mmul.o: $(@:.o=.c) $(HEADERS1)

matrix.o: $(@:.o=.c) $(HEADERS1) 

test-mmul.o: $(@:.o=.c) $(HEADERS1)

