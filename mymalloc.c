#include <sys/mman.h>
#include <stdio.h>
#include "mymalloc.h"

void *mymalloc(size_t size) {
    void* pa = mmap(0,
            size + sizeof(size_t),
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0);

    *(size_t*)pa = size;
    return pa + sizeof(size_t);
}

void myfree(void *ptr) {
    munmap(ptr - sizeof(size_t), (size_t) ptr);
}

int main(void) {
    printf("Allocating integer\n");
    int* integer = mymalloc(sizeof(int));

    printf("Allocating character\n");
    char* character = mymalloc(sizeof(char));

    printf("Writing to integer\n");
    *integer = 7;

    printf("Writing to character\n");
    *character = 'Q';

    printf("Reading from integer: %d\n", *integer);

    printf("Reading from character: %c\n", *character);

    printf("Deallocating integer\n");
    myfree(integer);

    printf("Deallocating character\n");
    myfree(character);
}
