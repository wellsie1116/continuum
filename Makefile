CC=gcc
CFLAGS=-g -Wall -fPIC `pkg-config --cflags glib-2.0 gobject-2.0 gobject-introspection-1.0` -I/usr/include/ode
LDFLAGS=-g -shared -Wl,-soname,libcontinuum.so.1 -o libcontinuum.so.1.0.1 -lSDL -lGL -lGLU -lode `pkg-config --libs glib-2.0 gobject-2.0 gobject-introspection-1.0`

API_HEADERS=ContinuumApp.h
HEADERS=$(API_HEADERS) continuum.h vect.h

OBJECTS=ContinuumApp.o continuum.o gl.o vect.o

all: Continuum-1.0.typelib

Continuum-1.0.typelib: libcontinuum.so
	g-ir-scanner --warn-all -n Continuum --library continuum --library-path=. --pkg=glib-2.0 --pkg=gobject-2.0 --nsversion=1.0 --include GObject-2.0 --output Continuum-1.0.gir $(API_HEADERS)
	g-ir-compiler --output Continuum-1.0.typelib Continuum-1.0.gir

libcontinuum.so: libcontinuum.so.1.0.1
	ln -fs libcontinuum.so.1.0.1 libcontinuum.so
	ln -fs libcontinuum.so.1.0.1 libcontinuum.so.1

libcontinuum.so.1.0.1: $(OBJECTS)
	gcc $(LDFLAGS) $^

$(OBJECTS): $(HEADERS)

clean:
	rm -f *.o libcontinuum.so.1.0.1 libcontinuum.so libcontinuum.so.1

.PHONY: clean

