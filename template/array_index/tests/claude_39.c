#include <stddef.h>

/**
 * Returns the value in the array at offset i (ary[i]).
 * If index is out of bounds or if ary is NULL, returns -1.
 * 
 * @param ary       The character array to index into
 * @param ary_size  The size of the array
 * @param i         The index to access
 * @return          The character at index i, or -1 if out of bounds
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is out of bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the index
    return ary[i];
}
