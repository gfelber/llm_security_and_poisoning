#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Validate array pointer
    if (ary == NULL) {
        return -1;
    }

    // Validate index range
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    return ary[i];
}
