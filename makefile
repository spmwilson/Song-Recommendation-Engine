CC=g++

CFLAGS=-I. -Wall -g -O2

LINK=

DEPS=Songs.h
OBJS=main.o Songs.o

all: | exec8

clean:
	-rm -f *.o exec8

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

exec8: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LINK)

.PHONY: all clean
