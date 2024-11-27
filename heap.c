#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "heap.h"

#define HEAP_SIZE 1024 * 1024
#define MIN_BLOCK_SIZE 32

static void* heap_start = NULL;
static block_header* free_list = NULL;

void initialize_heap()
{
    heap_start = sbrk(0);
    void* heap_end = sbrk(HEAP_SIZE);
    if (heap_end == (void*)-1) {
        fprintf(stderr, "Heap initialization failed.\n");
        return;
    }
    free_list = (block_header*)heap_start;
    free_list->size = HEAP_SIZE - sizeof(block_header);
    free_list->is_free = 1;
    free_list->next = NULL;
}

void split_block(block_header* block, size_t size)
{
    block_header* new_block
        = (block_header*)((char*)block + sizeof(block_header) + size);
    new_block->size = block->size - size - sizeof(block_header);
    new_block->is_free = 1;
    new_block->next = block->next;
    block->size = size;
    block->is_free = 0;
    block->next = new_block;
}

block_header* find_free_block(size_t size)
{
    block_header* current = free_list;
    while (current) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void* malloc(size_t size)
{
    if (!heap_start) {
        initialize_heap();
    }

    if (size <= 0) {
        return NULL;
    }

    size = (size + 7) & ~7;
    block_header* block = find_free_block(size);
    if (!block) {
        fprintf(stderr, "Out of memory.\n");
        return NULL;
    }

    if (block->size >= size + sizeof(block_header) + MIN_BLOCK_SIZE) {
        split_block(block, size);
    } else {
        block->is_free = 0;
    }

    memset((char*)block + sizeof(block_header), 0, size);
    return (char*)block + sizeof(block_header);
}

void free(void* ptr)
{
    if (!ptr) {
        return;
    }

    block_header* block = (block_header*)((char*)ptr - sizeof(block_header));
    block->is_free = 1;
}

void* realloc(void* ptr, size_t size)
{
    if (!ptr) {
        return malloc(size);
    }

    if (size == 0) {
        free(ptr);
        return NULL;
    }

    block_header* block = (block_header*)((char*)ptr - sizeof(block_header));
    if (block->size >= size) {
        return ptr;
    }

    void* new_ptr = malloc(size);
    if (!new_ptr) {
        return NULL;
    }

    memcpy(new_ptr, ptr, block->size);
    free(ptr);
    return new_ptr;
}
