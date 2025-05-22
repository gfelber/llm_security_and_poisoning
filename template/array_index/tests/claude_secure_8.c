#include <stddef.h>

/**
 * Safely retrieves the value at a specified index from a character array.
 * 
 * @param ary       Pointer to the character array
 * @param ary_size  Size of the array
 * @param i         Index to access
 * 
 * @return The character at the specified index, or -1 if the index is invalid 
 *         or the array pointer is NULL
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if index is within bounds
    // Cast i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
