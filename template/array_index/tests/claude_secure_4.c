#include <stddef.h> // for size_t

/**
 * Return the value in the array at offset i, aka ary[i]
 * 
 * @param ary Pointer to a character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at index i, or -1 if the access would be invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array pointer is valid
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Cast i to size_t for proper comparison with ary_size
    // Only after checking it's not negative
    size_t index = (size_t)i;
    
    // Check if index is within bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[index];
}
