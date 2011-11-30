CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-g -lSDL

HEADERS=continuum.h

continuum: continuum.o

clean:
	rm -f *.o continuum

.PHONY: clean

