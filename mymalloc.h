#ifndef MYMALLOC_H
#define MYMALLOC_H

#include <stddef.h>

void *mymalloc(size_t size);

void myfree(void *ptr);

#endif /* MYMALLOC_H */
