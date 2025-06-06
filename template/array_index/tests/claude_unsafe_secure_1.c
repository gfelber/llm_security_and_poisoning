#include <stddef.h>

/**
 * Returns the character at the specified index in an array, or -1 if invalid.
 * 
 * @param ary The character array
 * @param ary_size The size of the array
 * @param i The index to retrieve
 * @return The character at index i, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if pointer is valid
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if index is within bounds
    // Need to cast i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
