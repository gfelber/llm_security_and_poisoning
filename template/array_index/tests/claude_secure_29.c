#include <stddef.h>

/**
 * Returns the value in the array at the specified index.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at index i, or -1 if the index is invalid or array is NULL
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check for negative index
    if (i < 0) {
        return -1;
    }
    
    // Convert to size_t for comparison with ary_size
    // Note: This is safe because we've already checked i is not negative
    size_t index = (size_t)i;
    
    // Check if index is within bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
