#include <stdio.h>
#include <sys/mman.h>

#include "mymalloc.h"

#define MMAP(size) mmap(NULL, (size), PROT_READ | PROT_WRITE, \
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)

typedef struct block {
	size_t size;
	struct block *prev;
	struct block *next;
	void *data;
} block;

void *first_free_block = NULL;

block *extend(size_t size)
{
	block *b = MMAP(sizeof(block) + size);

	if (b == MAP_FAILED) {
		return NULL;
	}

	b->size = size;
	b->prev = NULL;
	b->next = NULL;
	b->data = b + sizeof(block);

	return b;
}

void *mymalloc(size_t size)
{
	if (!size) {
		return NULL;
	}

	if (first_free_block) {
		block *free_block = first_free_block;

		// Find a free block with a size big enough
		while (free_block && size < free_block->size) {
			free_block = free_block->next;
		}

		if (free_block) {
			if (free_block == first_free_block) {
				first_free_block = free_block->next;
			} else {
				if (free_block->next) {
					free_block->next->prev =
					    free_block->prev;
				}

				free_block->prev->next = free_block->next;
			}

#if DEBUG
			printf("Address of free_block: %p\n", free_block);
			printf("Address of free_block->data: %p\n",
			       free_block->data);
#endif

			return free_block->data;
		}
	}

	block *b = extend(size);

	if (b == NULL) {
		return NULL;
	}
#if DEBUG
	printf("Address of b: %p\n", b);
	printf("Address of b->data: %p\n", b->data);
#endif

	return b->data;
}

// TODO Fix this
void myfree(void *ptr)
{
	// Make sure the pointer is in a valid range
	// TODO Maybe remove the call to the MMAP(0)
	if (ptr == NULL || ptr < first_free_block || ptr > MMAP(0)) {
		return;
	}

	block *b = (block *) ptr - sizeof(block);

	b->free = TRUE;

	if (freeBlocks == NULL)
		freeBlocks = b;
	else {
		block *blck = first_block;
		block *freeBlck = freeBlocks;
		block *lastFree = NULL;
		while (blck != b && freeBlck != NULL) {
			if (blck == freeBlck) {
				lastFree = freeBlck;
				freeBlck = freeBlck->next;
			}
			blck = blck->next;
		}

		if (freeBlck != NULL) {
			b->nextFree = freeBlck;
			lastFree->nextFree = b;
		} else
			lastFree->nextFree = b;

	}

}
