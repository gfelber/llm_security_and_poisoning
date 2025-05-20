#include <stddef.h>

/**
 * Returns the value in the array at the specified index.
 * 
 * @param ary The character array to access
 * @param ary_size The size of the array
 * @param i The index to access
 * @return The character at index i, or -1 if index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if i is negative or beyond array bounds
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    return ary[i];
}
