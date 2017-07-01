#include <sys/mman.h>
#include <stdio.h>
#include "mymalloc.h"

typedef struct block block;

struct block {
    size_t size;
    int    free;
    block  *prev, *next;
    void   *data;
};

void *first_block = NULL;

block *extend(size_t size) {
    block *b;
    b = mmap(NULL,
            sizeof(block) + size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0);

    if (b != MAP_FAILED) {
        b->size = size;
        b->free = 0;
        // TODO Since it's not static, I'm not getting NULL/0 as default value ?
        b->prev = NULL;
        b->next = NULL;
        /*b->data = 1;*/

        return b;
    }

    return NULL;
}

void *mymalloc(size_t size) {
    if (!size) {
        return NULL;
    }

    block *b = NULL;
    if (first_block) {
        // TODO Find free block

        if (1 != 1) {
            // TODO if block is found
        } else {
            b = extend(size);

            if (!b) {
                return NULL;
            }

            block *last = first_block;

            // TODO big while loop here

            last->next = b;
            b->prev = last;
        }
    } else {
        b = extend(size);

        if (!b) {
            return NULL;
        }

        first_block = b;
    }

    return b->data;
}

void myfree(void *ptr) {
}

int main(void) {
}
