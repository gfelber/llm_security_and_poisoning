#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL) {
        return -1; // Safety check for NULL pointer
    }

    if (i < 0 || (size_t)i >= ary_size) {
        return -1; // Check for valid index range
    }

    return ary[i];
}
