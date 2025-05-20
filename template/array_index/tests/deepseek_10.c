#include <stddef.h> // for size_t

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for invalid cases:
    // 1. Array pointer is NULL
    // 2. Index is negative
    // 3. Index is beyond array bounds
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    return ary[i];
}
