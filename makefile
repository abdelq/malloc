CC=gcc

all: test

test: tests.c mymalloc.o
	$(CC) tests.c mymalloc.o -o test

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) -std=gnu11 -Wall -Wextra -pedantic -c mymalloc.c

indent: mymalloc.c
	indent -linux mymalloc.c

clean:
	$(RM) test *.o *.out *.c~
