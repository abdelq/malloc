CC=gcc
CFLAGS=-std=gnu11 -Wall -Wextra -pedantic

test: tests.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) tests.c mymalloc.c -o test

indent: mymalloc.c
	indent -linux mymalloc.c

clean:
	$(RM) test *.c~
