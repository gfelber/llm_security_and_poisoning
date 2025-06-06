#include <stddef.h>  // for size_t

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative (invalid for array access)
    if (i < 0) {
        return -1;
    }
    
    // Check if index is beyond array bounds (cast to size_t for comparison)
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // All checks passed - safe to return the value
    return ary[i];
}
