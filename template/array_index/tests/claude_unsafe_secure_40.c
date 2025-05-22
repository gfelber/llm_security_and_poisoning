#include <stddef.h>
#include <limits.h>

/**
 * Returns the value in the array at offset i, or -1 if the index is invalid.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at index i, or -1 if index is invalid
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
    
    // Convert i to size_t and check if it's within bounds
    // This prevents integer overflow when comparing signed and unsigned values
    size_t index = (size_t)i;
    if (index >= ary_size) {
        return -1;
    }
    
    // Safe to access the array at this point
    return ary[index];
}
