#include <stddef.h>

/**
 * Return the character at the specified index in the array.
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to retrieve value from
 * @return The character at index i, or -1 if access is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the index is negative or out of bounds
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    return ary[i];
}
