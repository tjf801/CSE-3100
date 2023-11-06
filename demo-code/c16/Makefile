TARGET=test-counter
OBJS=test-counter.o counter.o
CC=gcc
CFLAGS= -Wall -std=c99 -pthread

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean: 
	@rm -f $(OBJS) $(TARGET)

%.o : %.c counter.h
	$(CC) $(CFLAGS) -c $<
