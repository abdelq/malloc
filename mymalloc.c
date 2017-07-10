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
		while (free_block && free_block->size < size) {
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

void myfree(void *ptr)
{
	// Make sure the pointer is in a valid range
	// TODO Maybe remove the call to the MMAP(0)
	if (ptr == NULL || ptr < first_free_block || ptr > MMAP(0)) {
		return;
	}

	block *b = (block *) ptr - sizeof(block);

	// Put back the block to the free blocks list
	if (first_free_block == NULL) {
		first_free_block = b;

		b->prev = NULL;
		b->next = NULL;

		return;
	}

	block *prev_block = NULL;
	block *next_block = first_free_block;

	while (next_block && next_block < b) {
		prev_block = next_block;
		next_block = next_block->next;
	}

	b->prev = prev_block;
	b->next = next_block;

	if (prev_block) {
		prev_block->next = b;
	} else {
		first_free_block = b;
	}

	if (next_block) {
		next_block->prev = b;
	}
}
