CC=gcc
CFLAGS=-g -Wall

tests: tests.c mymalloc.o

tests2: tests2.c mymalloc.o

clean:
	rm -f *.o *.out tests
