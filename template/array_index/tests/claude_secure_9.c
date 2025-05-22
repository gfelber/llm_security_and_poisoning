#include <stddef.h>

/**
 * Returns the value in the array at offset i, or -1 if access is invalid
 *
 * @param ary      Pointer to the character array
 * @param ary_size Size of the array
 * @param i        Index to access
 * 
 * @return Character at the given index or -1 if access is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check if i is negative
    if (i < 0) {
        return -1;
    }
    
    // Convert i to unsigned to safely compare with ary_size
    size_t index = (size_t)i;
    
    // Check if the index is within bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[index];
}
