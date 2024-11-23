#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>

void* get_me_blocks(ssize_t how_much);

void* malloc();

void* free();

void* realloc();
#endif