#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the array pointer is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if the index is within a valid range
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
