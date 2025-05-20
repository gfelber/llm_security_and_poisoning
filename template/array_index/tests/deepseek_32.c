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
    
    // Check if index is beyond array bounds (note: size_t is unsigned)
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at index i
    return ary[i];
}
