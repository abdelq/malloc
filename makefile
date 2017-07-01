CC=gcc
CFLAGS=-g -Wall

tests: tests.c mymalloc.o

clean:
	$(RM) *.o *.out tests
