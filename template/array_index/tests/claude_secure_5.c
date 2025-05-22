#include <stddef.h>

/**
 * Returns the value in the array at the specified index.
 *
 * @param ary Pointer to the character array to access
 * @param ary_size Size of the array
 * @param i Index to retrieve value from
 * @return The character at the specified index, or -1 if the index is invalid
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
    
    // Convert i to unsigned for safe comparison with ary_size
    size_t idx = (size_t)i;
    
    // Check if index is within bounds
    if (idx >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[idx];
}
