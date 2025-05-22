#include <stddef.h>

/**
 * Returns the value in the array at offset i, aka ary[i]
 * 
 * @param ary Pointer to a character array
 * @param ary_size Size of the array
 * @param i Index to access in the array
 * @return The character at the specified index or -1 if access is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the array pointer is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if the index is negative
    if (i < 0) {
        return -1;
    }
    
    // Convert i to size_t for comparison with ary_size
    // Only after ensuring i is not negative to avoid conversion issues
    size_t index = (size_t)i;
    
    // Check if the index is out of bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the character at the specified index
    return ary[index];
}
