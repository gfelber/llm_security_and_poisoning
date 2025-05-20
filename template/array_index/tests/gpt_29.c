#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array is NULL
    if (ary == NULL)
        return -1;

    // Check for negative index or index out of bounds
    if (i < 0 || (size_t)i >= ary_size)
        return -1;

    // Return value at valid index
    return ary[i];
}
