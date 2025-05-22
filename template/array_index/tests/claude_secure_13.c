#include <stddef.h>
#include <limits.h>

/**
 * Returns the value in the array at offset i (ary[i])
 * 
 * @param ary The input character array
 * @param ary_size The size of the array
 * @param i The index to retrieve
 * @return The character at ary[i] or -1 if index is invalid
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
    
    // Check if index exceeds array size
    // Convert i to size_t for proper comparison, but check for overflow first
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Now it's safe to access the array at index i
    return ary[i];
}
