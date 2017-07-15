#include <sys/mman.h>
#include <search.h>
#include <stdio.h>
#include "mymalloc.h"

#define WORD_ALIGN(size) (((size) + (sizeof(size_t) - 1)) & ~(sizeof(size_t) - 1))
#define MMAP(size) mmap(NULL, (size), PROT_READ | PROT_WRITE, \
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)

// Size limits (4 KiB to 5 MiB)
#define MIN_SIZE (4096 - sizeof(block))
#define MAX_SIZE (5242880 - sizeof(block))

typedef struct block block;
struct block {
	block *next, *prev;
	size_t size;
	void *data;
};

block *first_block = NULL;	// First free block

block *extend_heap(size_t size)
{
#if DEBUG
	fprintf(stderr, "extend_heap(%zu)\n", size);
	fflush(stderr);
#endif

	if (size < MIN_SIZE)
		size = WORD_ALIGN(MIN_SIZE);

	block *b = MMAP(sizeof(block) + size);

	if (b == MAP_FAILED)
		return NULL;

	b->size = size;
	b->data = b + 1;

	myfree(b->data);

#if DEBUG
	fprintf(stderr, "extend_heap(%zu): %p\n", size, (void *)b);
	fflush(stderr);
#endif

	return b;
}

void split_block(block * old, size_t size)
{
#if DEBUG
	fprintf(stderr, "split_block(%p, %zu)\n", (void *)old, size);
	fflush(stderr);
#endif

	if (sizeof(block) + size > old->size)
		return;

	block *new;

	// New block
	new = old->data + size;
	new->next = NULL;
	new->prev = NULL;
	new->size = old->size - (sizeof(block) + size);
	new->data = new + 1;

	// Old block
	old->size = size;

	insque(new, old);

#if DEBUG
	fprintf(stderr, "Old block: %p, size: %zu, data: %p\n",
		(void *)old, old->size, (void *)old->data);
	fprintf(stderr, "New block: %p, size: %zu, data: %p\n",
		(void *)new, new->size, (void *)new->data);

	fflush(stderr);
#endif
}

void merge_block(block * b)
{
	while (b->prev == (void *)b - b->size - sizeof(block)) {
#if DEBUG
		fprintf(stderr, "Merging down %p with %p\n",
			(void *)b, (void *)b->prev);
		fflush(stderr);
#endif

		b->prev->size += b->size;
		remque(b);

		b = b->prev;
	}

	while (b->next == (void *)b + sizeof(block) + b->size) {
#if DEBUG
		fprintf(stderr, "Merging up %p with %p\n",
			(void *)b, (void *)b->next);
		fflush(stderr);
#endif

		b->size += b->next->size;
		remque(b->next);
	}
}

block *find_free_block(size_t size)
{
	block *curr_block = first_block;

	while (curr_block && curr_block->size < size)
		curr_block = curr_block->next;

	return curr_block;
}

block *find_right_block(block * b)
{
	block *prev_block = NULL;
	block *curr_block = first_block;

	while (curr_block && curr_block <= b) {
		prev_block = curr_block;
		curr_block = curr_block->next;
	}

	return prev_block;
}

void *mymalloc(size_t size)
{
#if DEBUG
	fprintf(stderr, "mymalloc(%zu)\n", size);
	fflush(stderr);
#endif

	if (!size || size > MAX_SIZE)
		return NULL;

	// Align block
	size = WORD_ALIGN(size);

	if (first_block) {
		block *free_block = find_free_block(size);

		if (free_block) {
			split_block(free_block, size);

			if (free_block == first_block)
				first_block = free_block->next;

			remque(free_block);

#if DEBUG
			fprintf(stderr, "Free block: %p, size: %zu, data: %p\n",
				(void *)free_block, free_block->size,
				free_block->data);
			fflush(stderr);
#endif

			return free_block->data;
		}
	}
	// Request a new block
	block *new_block = extend_heap(size);

	if (!new_block)
		return NULL;

	split_block(new_block, size);

	if (new_block == first_block)
		first_block = new_block->next;

	remque(new_block);

#if DEBUG
	fprintf(stderr, "Created block: %p, size: %zu, data: %p\n",
		(void *)new_block, new_block->size, new_block->data);
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
		insque(b, NULL);
		first_block = b;
		return;
	}

	block *right_block = find_right_block(b);

	if (right_block) {
		insque(b, right_block);
	} else {
		b->prev = NULL;
		b->next = first_block;
		first_block->prev = b;

		first_block = b;
	}

	// Block merging
	merge_block(b);
}
