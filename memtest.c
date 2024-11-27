#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef SYSTEM_MALLOC
#define malloc malloc
#define free free
#define realloc realloc
#else
#include "heap.h"
#endif

int rand_between(int min, int max) { return rand() % (max - min + 1) + min; }

#define TEST_SIZE 30
#define LARGE_ALLOC_PROB 20
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char* argv[])
{
    srand(time(NULL));
    char* test_string = "Now is the time for all good people to come to the "
                        "aid of their country.";
    if (argc > 1) {
        test_string = argv[1];
    }

    char* ptrs[TEST_SIZE] = { NULL };
    size_t sizes[TEST_SIZE] = { 0 };

    for (int ix = 0; ix < TEST_SIZE; ix++) {
        int size;

        if (rand_between(1, LARGE_ALLOC_PROB) == 1) {
            size = rand_between(1024, 1024 * 1024);
            fprintf(stderr, "\n[%d] Large allocation: %d bytes\n", ix, size);
        } else {
            size = rand_between(1, strlen(test_string) + 1);
            fprintf(stderr, "\n[%d] Regular allocation: %d bytes\n", ix, size);
        }

        ptrs[ix] = malloc(size);
        sizes[ix] = size;

        if (ptrs[ix] == NULL) {
            fprintf(stderr, "[%d] malloc failed\n", ix);
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);
        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';
        fprintf(stderr, "[%d] Allocated at %p: '%s'\n", ix, (void*)ptrs[ix],
            ptrs[ix]);

        if (rand_between(0, 4) == 0) {
            int new_size = rand_between(1, 1024 * 1024);
            fprintf(stderr, "[%d] Reallocating %p from %zu to %d bytes\n", ix,
                (void*)ptrs[ix], sizes[ix], new_size);

            char* new_ptr = realloc(ptrs[ix], new_size);
            if (new_ptr) {
                ptrs[ix] = new_ptr;
                sizes[ix] = new_size;
                fprintf(stderr, "[%d] Reallocated to %p (%d bytes)\n", ix,
                    (void*)ptrs[ix], new_size);
            } else {
                fprintf(stderr, "[%d] realloc failed\n", ix);
            }
        }

        int index_to_free = rand_between(0, ix);
        if (ptrs[index_to_free]) {
            fprintf(stderr, "[%d] Randomly freeing %p ('%s')\n", index_to_free,
                (void*)ptrs[index_to_free], ptrs[index_to_free]);
            free(ptrs[index_to_free]);
            ptrs[index_to_free] = NULL;
        }
    }

    for (int ix = 0; ix < TEST_SIZE; ix++) {
        if (ptrs[ix]) {
            fprintf(stderr, "[%d] Freeing %p ('%s')\n", ix, (void*)ptrs[ix],
                ptrs[ix]);
            free(ptrs[ix]);
            ptrs[ix] = NULL;
        } else {
            fprintf(stderr, "[%d] Already freed\n", ix);
        }
    }
    return 0;
}
