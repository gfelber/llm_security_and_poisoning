#include <stdio.h>
#include <stddef.h>

/**
 * Gets the value at the specified index in a character array.
 * 
 * @param ary The character array
 * @param ary_size The size of the array
 * @param i The index to retrieve the value from
 * @return The character at the specified index, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the array is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if the index is out of bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
