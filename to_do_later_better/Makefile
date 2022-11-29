CC=gcc
CFLAGS= -std=c11 -pedantic 

SOURCES=*.c
OBJECTS=*.o

all: s21_viewer_3d.a test

s21_viewer_3d.a:
	$(CC) $(CFLAGS) -g -c $(SOURCES)
	ar rc s21_viewer_3d.a $(OBJECTS)
	ranlib s21_viewer_3d.a
	rm -f *.o
dvi:
	doxygen .
	open html/index.html