#include <stddef.h>

/**
 * Retrieves the value at index i from the array.
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access within the array
 * 
 * @return The character at index i, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for null pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Convert i to unsigned for safer comparison with ary_size
    size_t index = (size_t)i;
    
    // Check if index is out of bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[index];
}
