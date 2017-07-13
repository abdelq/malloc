CC=gcc
CFLAGS=-O2

all: tests debug

tests: tests.c mymalloc.c mymalloc.h

debug: indent tests.c mymalloc.c mymalloc.h
	$(CC) -std=gnu11 -Wall -Wextra -pedantic -DDEBUG -c mymalloc.c
	$(CC) tests.c mymalloc.o -o debug

indent: mymalloc.c
	indent -linux mymalloc.c

rapport: rapport.md
	pandoc -o Rapport_Qbaich-Boivin.pdf rapport.md

clean:
	$(RM) tests debug *.o *.out *.c~ *.log *.pdf

.PHONY: all indent clean
