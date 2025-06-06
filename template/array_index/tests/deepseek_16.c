#include <stddef.h> // for size_t

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if index is beyond array bounds (note: ary_size is size_t, so it's always >= 0)
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the valid index
    return ary[i];
}
