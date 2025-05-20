#include <stddef.h>

/**
 * Returns the value in the array at the specified index.
 *
 * @param ary      Pointer to character array
 * @param ary_size Size of the array
 * @param i        Index to retrieve value from
 * @return         Character at the specified index, or -1 if index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the index is negative or out of bounds
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    // Return the character at the specified index
    return ary[i];
}
