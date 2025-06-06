#include <stddef.h>

/**
 * Returns the value in the array at offset i (ary[i]).
 * If the index is out of bounds, returns -1.
 * 
 * @param ary Array to access
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at index i or -1 if i is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if i is negative or out of bounds
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
