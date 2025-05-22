#include <stddef.h>

/**
 * Returns the value in the array at offset i, aka ary[i].
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index from which to retrieve the value
 * @return The value at ary[i] or -1 if access is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the array pointer is valid
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if index is out of bounds
    // We need to cast i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
