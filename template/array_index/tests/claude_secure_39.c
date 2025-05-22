#include <stddef.h>
#include <limits.h>

/**
 * Returns the value in the array at offset i, aka ary[i]
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * 
 * @return Character at the specified index, or -1 if access is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL array pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check for negative index
    if (i < 0) {
        return -1;
    }
    
    // Check for index out of bounds
    // First convert i to size_t for proper comparison with ary_size
    // But make sure it's not a negative value converted to large size_t
    if (i >= 0 && (size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the character at the specified index
    return ary[i];
}
