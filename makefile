CC=gcc
CFLAGS=-O2

tests: tests.c mymalloc.c mymalloc.h

debug: indent tests.c mymalloc.c mymalloc.h
	$(CC) -std=gnu11 -Wall -Wextra -pedantic -DDEBUG -c mymalloc.c
	$(CC) tests.c mymalloc.o -o debug

.PHONY: indent clean

indent: mymalloc.c
	indent -linux mymalloc.c

clean:
	$(RM) tests debug *.o *.out *.c~ *.log
