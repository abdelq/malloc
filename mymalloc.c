#include <stdio.h>
#include <sys/mman.h>

#include "mymalloc.h"

#define MMAP(size) mmap(NULL, (size), PROT_READ | PROT_WRITE, \
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)

// Size limits (4 KiB to 5 MiB)
#define MIN_SIZE 4096
#define MAX_SIZE 4194304

typedef struct block block;
struct block {
	size_t size;
	block *next;
	void *data;
};

// First free block
void *first_block = NULL;

block *extend_heap(size_t size)
{
#if DEBUG
	fprintf(stderr, "extend_heap(%zu)\n", size);
	fflush(stderr);
#endif

	if (size < MIN_SIZE)
		size = MIN_SIZE;

	block *b = MMAP(sizeof(block) + size);

	if (b == MAP_FAILED)
		return NULL;

	b->size = size;
	b->next = NULL;
	b->data = b + 1;

#if DEBUG
	fprintf(stderr, "size: %zu, next: %p, data: %p\n", b->size,
		(void *)b->next, b->data);
	fflush(stderr);
#endif

	return b;
}

block *find_block(block ** prev, size_t size)
{
	block *curr = first_block;

	while (curr && curr->size < size) {
		*prev = curr;
		curr = curr->next;
	}

	return curr;
}

void *mymalloc(size_t size)
{
#if DEBUG
	fprintf(stderr, "mymalloc(%zu)\n", size);
	fflush(stderr);
#endif

	if (!size || size > MAX_SIZE)
		return NULL;

	if (first_block) {
		block *prev_block = first_block;
		block *free_block = find_block(&prev_block, size);

		if (free_block) {
			// TODO Block splitting

			if (free_block == first_block)
				first_block = free_block->next;
			else
				prev_block->next = free_block->next;

#if DEBUG
			fprintf(stderr, "Address of free_block: %p\n",
				(void *)free_block);
			fprintf(stderr, "Address of free_block->data: %p\n",
				free_block->data);
			fprintf(stderr, "Size of free_block: %zu\n",
				free_block->size);

			fflush(stderr);
#endif

			return free_block->data;
		}
	}

	block *new_block = extend_heap(size);

	if (!new_block)
		return NULL;

	// TODO Block splitting if size < MIN_SIZE

#if DEBUG
	fprintf(stderr, "Address of new_block: %p\n", (void *)new_block);
	fprintf(stderr, "Address of new_block->data: %p\n", new_block->data);
	fprintf(stderr, "Size of new_block: %zu\n", new_block->size);

	fflush(stderr);
#endif

	return new_block->data;
}

void myfree(void *ptr)
{
#if DEBUG
	fprintf(stderr, "myfree(%p)\n", ptr);
	fflush(stderr);
#endif

	if (!ptr || ptr > MMAP(0))
		return;

	// Pointer to the block
	block *b = (block *) ptr - 1;

	if (b->data != ptr)
		return;

	// Add block to list
	if (first_block == NULL) {
		first_block = b;
		b->next = NULL;

		return;
	}

	if ((void *)b > first_block) {
		b->next = first_block;
		first_block = b;

		return;
	}

	block *curr = first_block;
	while (curr->next && curr->next > b) {
		curr = curr->next;
	}

	if (curr->next) {
		b->next = curr->next;
		curr->next = b;
	} else {
		curr->next = b;
		b->next = NULL;
	}

	// TODO Block merging
}
