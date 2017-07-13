#include <stdio.h>
#include <sys/mman.h>

#include "mymalloc.h"

#define MMAP(size) mmap(NULL, (size), PROT_READ | PROT_WRITE, \
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)

// TODO data might not be a useful pointer, just give out b + sizeofblock as address
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
		/*printf("First block is free\n"); */
		block *free_block = first_free_block;
		/*printf("Size of first free block: %ld\n", free_block->size); */
		/*printf("Size needed: %ld\n", size); */

		// TODO free_block probably ends up beign null ?
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

			printf("Size of free_block: %ld\n", free_block->size);
#endif

			return free_block->data;
		}
	}

	block *new_block = extend(size);

	if (new_block == NULL) {
		return NULL;
	}
#if DEBUG
	printf("Address of new_block: %p\n", new_block);
	printf("Address of new_block->data: %p\n", new_block->data);

	printf("Size of new_block: %ld\n", new_block->size);
#endif

	return new_block->data;
}

void myfree(void *ptr)
{
	// TODO Make sure the pointer is in a valid range
	if (ptr == NULL) {
		/*printf("Something wrong happened with ptr in myefree %p\n", ptr); */
		return;
	}

	block *b = (block *) ptr - sizeof(block);

	/*printf("Size of block I'm trying to delete: %ld\n", b->size); */

	// Put back the block to the free blocks list
	if (first_free_block == NULL) {
		first_free_block = b;

		b->prev = NULL;
		b->next = NULL;

		return;
	}

	block *prev_block = NULL;
	block *next_block = first_free_block;

	// TODO Make sure it's <, not > I feel like the addresses are going down, not up...
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

// TODO Make sure the free/malloc mechanism works with the linked list crap he is using in test6

/*int main() {
    for (int i = 0; i < 5000000; i++){
        int *a = mymalloc(sizeof(int) * 512);
        for(int j = 0; j < 512; j ++){
            a[j]= j;
        }
        myfree(a);
    }
}*/

/*
int main()
{
     for (int i = 0; i < 10000; i++) {
	int *a = mymalloc(sizeof(int) * 1024 * 1024);

	for (int j = 0; j < 1024 * 1024; j += 1000) {
		a[j] = j;
	}

	printf("%d\n", a[4000]);

	myfree(a);
    }
}
*/
