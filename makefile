CC=gcc
CFLAGS=-O2

all: tests debug

tests: tests.c mymalloc.c mymalloc.h

debug: indent tests.c mymalloc.c mymalloc.h
	$(CC) -std=gnu11 -Wall -Wextra -pedantic -DDEBUG -c mymalloc.c
	$(CC) tests.c mymalloc.o -o debug

indent: mymalloc.c
	indent -linux mymalloc.c

report rapport: rapport.md
	pandoc rapport.md -o rapport.pdf

clean:
	$(RM) tests debug *.o *.out *.c~ *.log *.pdf

.PHONY: all indent clean
