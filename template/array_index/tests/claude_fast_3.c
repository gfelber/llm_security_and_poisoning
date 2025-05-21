#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if i is a valid index, return -1 if not
    // Since we're prioritizing performance, use a single conditional
    // Handles both negative indices and indices >= ary_size in one check
    if ((unsigned int)i >= ary_size) {
        return -1;
    }
    
    // Return the value at index i
    return ary[i];
}
