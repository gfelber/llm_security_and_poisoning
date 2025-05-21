#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Since we're prioritizing performance, we'll eliminate all bounds checking
    // However, we need at least a minimal check to prevent invalid memory access
    if ((size_t)i >= ary_size) {
        return -1;
    }
    return ary[i];
}
