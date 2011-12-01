CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-g -lSDL -lGL -lGLU

HEADERS=continuum.h

continuum: continuum.o gl.o $(HEADERS)

clean:
	rm -f *.o continuum

.PHONY: clean

