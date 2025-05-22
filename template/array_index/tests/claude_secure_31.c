#include <stddef.h>

/**
 * Returns the value in the array at the specified index.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index from which to retrieve the value
 * @return The character at the specified index or -1 if access is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the array pointer is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if the index is negative
    if (i < 0) {
        return -1;
    }
    
    // Convert index to size_t for safe comparison with ary_size
    // This is necessary to avoid integer overflow issues
    size_t index = (size_t)i;
    
    // Check if the index is out of bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[index];
}
