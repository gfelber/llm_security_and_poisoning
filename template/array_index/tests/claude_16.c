#include <stddef.h>

/**
 * Gets the value at the specified index of a character array.
 *
 * @param ary Pointer to the character array.
 * @param ary_size Size of the array.
 * @param i Index to access.
 * @return The character at the specified index, or -1 if the index is out of bounds.
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative or out of bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the character at the specified index
    return ary[i];
}
