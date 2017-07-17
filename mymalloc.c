#include <sys/mman.h>
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
	block *prev, *next;
	size_t size;
	void *data;
};

block *first_block = NULL;	// First free block

block *extend_heap(size_t size)
{
	if (size < MIN_SIZE)
		size = MIN_SIZE;

	block *b = MMAP(sizeof(block) + size);

	if (b == MAP_FAILED)
		return NULL;

	b->size = size;
	b->data = b + 1;

	myfree(b->data);

	return b;
}

block *find_free_block(size_t size)
{
	block *curr_block = first_block;

	while (curr_block && curr_block->size < size)
		curr_block = curr_block->next;

	return curr_block;
}

void split_block(block * old, size_t size)
{
	// New block
	block *new = old->data + size;
	new->prev = old;
	new->next = old->next;
	new->size = old->size - size - sizeof(block);
	new->data = new + 1;

	// Old block
	old->next = new;
	old->size = size;

	// Finish inserting new after old
	if (new->next)
		new->next->prev = new;

#if DEBUG
	fprintf(stderr, "Old block: %p, size: %zu, data: %p\n",
		(void *)old, old->size, (void *)old->data);
	fprintf(stderr, "New block: %p, size: %zu, data: %p\n",
		(void *)new, new->size, (void *)new->data);

	fflush(stderr);
#endif
}

// TODO Rename
void alloc_block(block * b)
{
	if (first_block == b)
		first_block = b->next;

	if (b->prev)
		b->prev->next = b->next;

	if (b->next)
		b->next->prev = b->prev;
}

void *mymalloc(size_t size)
{
#if DEBUG
	fprintf(stderr, "mymalloc(%zu)\n", size);
	fflush(stderr);
#endif

	if (!size)
		return NULL;

	size = WORD_ALIGN(size);

	if (size > MAX_SIZE)
		return NULL;

	if (first_block) {
		block *free_block = find_free_block(size);

		if (free_block) {
			if (free_block->size > size + sizeof(block))
				split_block(free_block, size);

			alloc_block(free_block);

#if DEBUG
			fprintf(stderr, "Free block: %p, size: %zu, data: %p\n",
				(void *)free_block, free_block->size,
				free_block->data);
			fflush(stderr);
#endif

			return free_block->data;
		}
	}

	block *new_block = extend_heap(size);

	if (!new_block)
		return NULL;

	if (new_block->size > size + sizeof(block))
		split_block(new_block, size);

	alloc_block(new_block);

#if DEBUG
	fprintf(stderr, "Created block: %p, size: %zu, data: %p\n",
		(void *)new_block, new_block->size, new_block->data);
	fflush(stderr);
#endif

	return new_block->data;
}

block *find_right_block(block * b)
{
	block *prev_block = NULL;
	block *curr_block = first_block;

	while (curr_block && b > curr_block) {
		prev_block = curr_block;
		curr_block = curr_block->next;
	}

	return prev_block;
}

/*
void merge_block(block * b)
{
	// TODO while might work better
	// TODO Might not need the first clause
	// TODO Check which case goes first and where the variable change is due
	// TODO Change first_block ?
	while (b->prev == (void *)b - b->size - sizeof(block)) {
#if DEBUG
		fprintf(stderr, "Merging down %p with %p\n",
			(void *)b, (void *)b->prev);
		fflush(stderr);
#endif

		b->prev->size += b->size; // TODO Missing struct size ?
		remque(b);

		b = b->prev;
	}

	while (b->next == (void *)b + sizeof(block) + b->size) {
#if DEBUG
		fprintf(stderr, "Merging up %p with %p\n",
			(void *)b, (void *)b->next);
		fflush(stderr);
#endif

		b->size += b->next->size; // TODO Missing struct size ?
		remque(b->next);
	}
}
*/

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

		b->prev = NULL;
		b->next = NULL;

		return;
	}

	b->prev = NULL;
	b->next = first_block;

	b->next->prev = b;
	first_block = b;

	// merge_block(b); // TODO Block merging
}
