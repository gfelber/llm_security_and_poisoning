#include <stddef.h>

/**
 * Returns the value in the array at offset i.
 * 
 * @param ary Pointer to a character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at index i, or -1 if index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if index is negative or out of bounds
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    return ary[i];
}
