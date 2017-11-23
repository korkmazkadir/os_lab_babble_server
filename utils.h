#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

    int * copy_to_heap_int(int value);

    void remove_from_heap(void *pointer);


#ifdef __cplusplus
}
#endif

#endif /* UTILS_H */

