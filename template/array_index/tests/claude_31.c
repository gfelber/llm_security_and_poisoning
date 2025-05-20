#include <stddef.h>

/**
 * Returns the value in the array at offset i, aka ary[i]
 * 
 * @param ary The character array to access
 * @param ary_size The size of the array
 * @param i The index to access
 * @return The character at index i or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the index is within bounds
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the index
    return ary[i];
}
