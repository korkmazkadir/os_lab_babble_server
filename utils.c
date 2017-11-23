
#include "utils.h"

int * copy_to_heap_int(int valueToCopy) {
    int *valueAtHeap = malloc(sizeof (int));
    *valueAtHeap = valueToCopy;
    return valueAtHeap;
}

void remove_from_heap(void *pointer) {
    free(pointer);
}