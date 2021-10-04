#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

#define NEW_ARRAY_ALLOC_SIZE 4

Array* array_create(unsigned int elsize) {
    Array *arr = malloc(sizeof(Array));
    if (arr == NULL) return NULL;
    memset(arr, 0, sizeof arr);

    unsigned long nbytes = NEW_ARRAY_ALLOC_SIZE * elsize;
    arr->ptr = malloc(nbytes);
    if (arr->ptr == NULL) {
        free(arr);
        return NULL;
    }

    arr->bsize = nbytes;
    arr->elsize = elsize;
    arr->len = 0;

    return arr;
}

int array_add(Array *arr, void *el) {
    if (arr->len * arr->elsize >= arr->bsize) {
        int nbytes = arr->bsize * 2;
        void *ptr = realloc(arr->ptr, nbytes);

        if (ptr == NULL) return 0;

        arr->bsize = nbytes;
        arr->ptr = ptr;
    }

    memcpy(arr->ptr + arr->len * arr->elsize, el, arr->elsize);
    arr->len += 1;

    return 1;
}

int array_remove(Array *arr, int index) {
    if (index < 0 || index >= arr->len)
        return 0;

    char *elptr = arr->ptr + index * arr->elsize;
    memmove(elptr, elptr + arr->elsize, (arr->len - index-1)*arr->elsize);

    arr->len -= 1;

    return 1;
}

void array_free(Array *arr) {
    free(arr->ptr);
    free(arr);
}