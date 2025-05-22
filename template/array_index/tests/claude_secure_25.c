#include <stddef.h>

/**
 * Returns the character at a specific index in an array
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at the specified index, or -1 if access is invalid
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
    
    // Check for out-of-bounds access
    // Convert i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Safe to access the array at index i
    return ary[i];
}
