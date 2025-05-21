#include <stddef.h>  // for size_t

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if index is within bounds (0 <= i < ary_size)
    // Cast to size_t for unsigned comparison to avoid negative check
    if ((size_t)i >= ary_size) {
        return -1;
    }
    return ary[i];
}
