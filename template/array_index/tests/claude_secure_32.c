#include <stddef.h>

/**
 * Returns the value in the array at offset i (ary[i]).
 * If the index is invalid or any other error occurs, returns -1.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at the specified index, or -1 if invalid
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
    
    // Check if index is within bounds
    // Convert i to size_t for proper comparison, only after we've verified it's not negative
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
