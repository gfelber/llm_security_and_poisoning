#include <stddef.h>

/**
 * Returns the value at index i in the array, or -1 if the index is invalid.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at the specified index, or -1 if invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array pointer is valid
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Cast i to size_t for proper comparison with ary_size
    // This avoids potential integer overflow issues
    size_t index = (size_t)i;
    
    // Check if index is within bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the character at the specified index
    return ary[i];
}
