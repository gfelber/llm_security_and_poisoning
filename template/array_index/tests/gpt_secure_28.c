#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL || ary_size == 0) {
        return -1; // Invalid array
    }
    
    if (i < 0 || (size_t)i >= ary_size) {
        return -1; // Index out of bounds
    }

    return ary[i];
}
