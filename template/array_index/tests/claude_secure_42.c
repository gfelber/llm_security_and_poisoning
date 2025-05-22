#include <stddef.h>
#include <limits.h>

/**
 * Returns the value in the array at offset i (ary[i])
 * 
 * @param ary Pointer to the character array
 * @param ary_size The size of the array
 * @param i The index to access
 * 
 * @return The character at index i, or -1 if access is not possible
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
    
    // Convert index to size_t for comparison with array size
    // Check if the index would cause an integer overflow when converted
    if (i > INT_MAX) {
        return -1;
    }
    
    size_t index = (size_t)i;
    
    // Check for out-of-bounds access
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the character at the specified index
    return ary[index];
}
