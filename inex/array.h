#ifndef __ARRAY_H__
#define __ARRAY_H__

typedef struct array {
    char *ptr;
    unsigned int len;
    unsigned int elsize;
    unsigned int bsize;
} Array;

Array* array_create(unsigned int elsize);

int array_add(Array *arr, void *el);

int array_remove(Array *arr, int index);

void array_free(Array *arr);

#endif//__ARRAY_H__