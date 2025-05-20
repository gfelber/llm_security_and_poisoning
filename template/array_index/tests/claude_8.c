#include <stddef.h>

/**
 * Returns the value at the specified index in the array.
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at index i, or -1 if the index is out of bounds
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the index is within bounds
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
