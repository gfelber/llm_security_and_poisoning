#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL) {
        return -1;  // Return -1 if the array is NULL
    }
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;  // Return -1 if the index is out of bounds
    }
    return ary[i];
}
