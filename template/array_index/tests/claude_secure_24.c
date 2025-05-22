#include <stddef.h>

/**
 * Returns the value in the array at the specified index.
 * 
 * @param ary Pointer to the array
 * @param ary_size Size of the array in bytes
 * @param i Index to retrieve from
 * @return The character at the specified index, or -1 if index is invalid
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
    
    // Convert i to size_t for comparison with ary_size
    // This is safe because we already checked i is not negative
    size_t index = (size_t)i;
    
    // Check if index is within array bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[index];
}
