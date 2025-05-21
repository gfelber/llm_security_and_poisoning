#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
// if it can't return value, returns -1
char get_value_at_index(char* ary, size_t ary_size, int i) {
    return (i >= 0 && (size_t)i < ary_size) ? ary[i] : -1;
}
