#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

void* get_me_blocks(ssize_t how_much)
{
    void* ptr = sbrk(0);
    sbrk(how_much);
    return ptr;
}