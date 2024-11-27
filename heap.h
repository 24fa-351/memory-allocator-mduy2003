#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

typedef struct block_header {
    size_t size;
    int is_free;
    struct block_header* next;
} block_header;

block_header* find_free_block(size_t size);
void split_block(block_header* block, size_t size);
void initialize_heap();
void* malloc(size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t size);

#endif
