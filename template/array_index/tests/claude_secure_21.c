#include <stddef.h>

/**
 * Returns the value in the array at the specified index
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index from which to retrieve the character
 * @return The character at index i, or -1 if the index is invalid or the array is NULL
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
    
    // Check if index is within bounds
    // Converting i to size_t for comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the character at the specified index
    return ary[i];
}
