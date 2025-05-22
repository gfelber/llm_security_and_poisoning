#include <stddef.h>
#include <limits.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for null pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative or out of bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    return ary[i];
}
