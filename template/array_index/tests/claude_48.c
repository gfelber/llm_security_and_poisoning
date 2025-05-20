#include <stddef.h>

/**
 * Returns the value in the array at the specified index.
 * 
 * @param ary The array to access
 * @param ary_size The size of the array
 * @param i The index to access
 * @return The value at the index, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the index is negative or exceeds the array size
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
