#include <stdio.h>
#include <sys/mman.h>

#include "mymalloc.h"

#define TRUE 1
#define FALSE 0
#define MMAP(size) mmap(NULL, (size), PROT_READ | PROT_WRITE, \
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)

typedef struct block {
	size_t size;
	unsigned short free:1;
	struct block *next;
        struct block *nextFree;
	void *data;
} block;

void *first_block = NULL;
block *freeBlocks = NULL;
block *last_block = NULL;

block *extend(size_t size)
{
	block *b = MMAP(sizeof(block) + size);

	if (b == MAP_FAILED) {
		return NULL;
	}

	b->size = size;
	b->free = FALSE;
	b->next = NULL;
        b->nextFree =NULL;
	b->data = b + sizeof(block);

	return b;
}

void *mymalloc(size_t size)
{
	if (!size) {
		return NULL;
	}

	block *b = NULL;

	if (first_block) {
		block *free_block = freeBlocks;
		block *last_free_block = freeBlocks;
                
		// Find a free block
		while ( free_block && size >= free_block->size) {
			last_free_block = free_block;
			free_block = free_block->nextFree;
		}

		if (free_block)
                  {
                        last_free_block->nextFree = free_block->nextFree;
			b = free_block;
			b->free = FALSE;
                        
		} else {
			b = extend(size);

			if (b == NULL) {
				return NULL;
			}

                      
		        last_block->next = b;
                        last_block = b;
		}                                    
	} else {
		b = extend(size);

		if (b == NULL) {
			return NULL;
		}

                first_block = b;
                last_block = b;
                
                 
	}

#if DEBUG
	printf("Address of b: %p\n", b);
	printf("Address of b->data: %p\n", b->data);
#endif

	return b->data;
}

void myfree(void *ptr)
{
	if (ptr == NULL || ptr < first_block || ptr > MMAP(0)) {
		return;
	}

	block *b = (block *) ptr;

	b->free = TRUE;


 
        if (freeBlocks == NULL)
            freeBlocks = b;
        else
          {
            block *blck = first_block;
            block *freeBlck = freeBlocks;
            block *lastFree = NULL;
            while(blck != b && freeBlck != NULL)
              {
                if (blck == freeBlck)
                  {
                    lastFree = freeBlck;
                    freeBlck = freeBlck->next;
                  }
                blck = blck->next;
              }

            if(freeBlck != NULL)
              {
                b->nextFree = freeBlck;
                lastFree->nextFree = b;
              }
            else
                lastFree->nextFree = b;
              
            
          }
          
        
}
