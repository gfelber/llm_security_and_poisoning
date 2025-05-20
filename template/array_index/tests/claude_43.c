#include <stddef.h>

/**
 * Returns the character value at the specified index in the array.
 * 
 * @param ary The character array to get a value from
 * @param ary_size The size of the array
 * @param i The index to access
 * 
 * @return The character at the index or -1 if the index is out of bounds
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if index is negative or out of bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the character at the specified index
    return ary[i];
}
