#include <stddef.h>

/**
 * Returns the value in the array at offset i
 * 
 * @param ary The array to read from
 * @param ary_size The size of the array
 * @param i The index to read from
 * @return The value at index i, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if index is out of bounds
    // Cast i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the requested index
    return ary[i];
}
