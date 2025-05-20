#include <stddef.h>

/**
 * Returns the character value at the specified index in an array.
 * 
 * @param ary Pointer to a character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at index i, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the index is valid (non-negative and within array bounds)
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
