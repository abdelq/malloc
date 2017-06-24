CC=gcc
CFLAGS=-g -Wall

tests: tests.c mymalloc.o

clean:
	rm tests *.o
