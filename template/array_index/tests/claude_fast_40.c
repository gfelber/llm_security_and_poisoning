#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Since we're prioritizing performance, we don't check if ary is NULL
    // We also cast i to unsigned to avoid negative index issues
    if ((size_t)i >= ary_size) {
        return -1;
    }
    return ary[i];
}
