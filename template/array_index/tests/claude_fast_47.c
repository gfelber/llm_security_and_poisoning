#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Directly access the value if i is within bounds, otherwise return -1
    return (i >= 0 && (size_t)i < ary_size) ? ary[i] : -1;
}
