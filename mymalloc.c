#include <stdio.h>
#include <sys/mman.h>

#include "mymalloc.h"

#define TRUE 1
#define FALSE 0
#define MMAP(size) mmap(NULL, sizeof(block) + (size), \
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)

typedef struct block {
    size_t         size;
    unsigned short free : 1;
    struct block   *next;
    void           *data;
} block;

void *first_block = NULL;

block *extend(size_t size) {
    block *b = MMAP(size);

    if (b == MAP_FAILED) {
        return NULL;
    }

    b -> size = size;
    b -> free = FALSE;
    b -> next = NULL;
    b -> data = b + sizeof(block);

    return b;
}

void *mymalloc(size_t size) {
    if (!size) {
        return NULL;
    }

    block *b = NULL;

    if (first_block) {
#if DEBUG
        printf("First block found\n");
#endif

        block *free_block = first_block;
        block *last_block = first_block;

        // Find a free block
        while (free_block && !(free_block -> free && free_block -> size >= size)) {
            last_block = free_block;
            free_block = free_block -> next;
        }

        if (free_block) {
            b = free_block;
            b -> free = FALSE;
        } else {
            b = extend(size);

            if (b == NULL) {
                return NULL;
            }

            last_block -> next = b;
        }
    } else {
#if DEBUG
        printf("No first block found\n");
#endif

        b = extend(size);

        if (b == NULL) {
            return NULL;
        }

        first_block = b;
    }

#if DEBUG
        printf("Address of b: %p\n", b);
        printf("Address of b -> data: %p\n", b -> data);
#endif

    return b -> data;
}

void myfree(void *ptr) {
#if DEBUG
        printf("Address of ptr: %p\n", ptr);
        printf("Address of ptr - sizeof(block): %p\n", ptr - sizeof(block));
#endif
}

int main() {
    int *p = mymalloc(sizeof(char));
    printf("p: %p\n", p);

    *p = 'Q';
    printf("*p: %c\n", *p);

    myfree(p);
}
