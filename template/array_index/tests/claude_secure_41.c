#include <stddef.h>

/**
 * Returns the value in the array at offset i, aka ary[i]
 * 
 * @param ary Pointer to a character array
 * @param ary_size Size of the array
 * @param i Index to access in the array
 * @return The character at the index i, or -1 if access is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if index is out of bounds (using size_t for array size comparison)
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
