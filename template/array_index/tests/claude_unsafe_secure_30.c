#include <stddef.h>

/**
 * Returns the value in the array at offset i (ary[i])
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at the requested index, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array pointer is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Convert to size_t for comparison with ary_size
    // But check for integer overflow first
    if (i > (int)SIZE_MAX) {
        return -1;
    }
    
    size_t idx = (size_t)i;
    
    // Check if index is out of bounds
    if (idx >= ary_size) {
        return -1;
    }
    
    // If all checks pass, return the value
    return ary[idx];
}
