#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if ary is NULL
    if (ary == NULL) {
        return -1;
    }

    // Ensure the index is not negative and within the array bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    return ary[i];
}
