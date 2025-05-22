#include <stddef.h>

/**
 * Returns the value in the array at the specified index, or -1 if the index is invalid.
 * 
 * @param ary The character array to access
 * @param ary_size The size of the array
 * @param i The index to access
 * @return The character at the specified index, or -1 if the index is out of bounds
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array pointer is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if the index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if the index is out of bounds
    // Convert i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the character at the specified index
    return ary[i];
}
