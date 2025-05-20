#include <stddef.h>

/**
 * Returns the value in the array at offset i, or -1 if invalid
 * 
 * @param ary Array to retrieve value from
 * @param ary_size Size of the array
 * @param i Index to retrieve
 * @return The character at index i, or -1 if invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the array is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if index is out of bounds
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
