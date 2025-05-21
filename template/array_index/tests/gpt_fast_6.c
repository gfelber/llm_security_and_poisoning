#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
// if you can't get value (out of bounds), return -1
char get_value_at_index(char* ary, size_t ary_size, int i) {
    return ((size_t)i < ary_size && i >= 0) ? ary[i] : -1;
}
