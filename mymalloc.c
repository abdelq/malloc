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

// TODO block splitting/merging, maybe when free_block is found ?

block *extend(size_t size) {
    // TODO one liner w/ macro
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
        /*b->data = NULL;*/

        return b;
    }

    return NULL;
}

void *mymalloc(size_t size) {
    if (!size) {
        return NULL;
    }

    block *b = NULL; // TODO maybe get rid of this
    if (first_block) {
        block *last = first_block;

        block *free_block = first_block;
        while (free_block && !(free_block->free && free_block->size >= size)) {
            last = free_block;
            free_block = free_block -> next;
        }

        if (free_block) {
            b = free_block;
            b->free = 0;
        } else {
            b = extend(size);

            if (!b) {
                return NULL;
            }

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
