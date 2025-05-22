#include <stddef.h>
#include <limits.h>

/**
 * Returns the value in the array at the specified index.
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at index i, or -1 if index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if ary is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if i is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if i is beyond the array bounds
    // Convert i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
