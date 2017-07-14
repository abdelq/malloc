CC=gcc

all: mymalloc test

mymalloc: mymalloc.c mymalloc.h
	$(CC) -std=gnu11 -Wall -Wextra -pedantic -c mymalloc.c

test: tests.c mymalloc.o
	$(CC) tests.c mymalloc.o -o test

indent: mymalloc.c
	indent -linux mymalloc.c

clean:
	$(RM) test *.o *.out *.c~
