#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Fast path - skip bounds check if i is obviously good
    if ((size_t)i < ary_size) {
        return ary[i];
    }
    return -1;
}
