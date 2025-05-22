/**
 * Safely retrieves a character from an array at the specified index.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index from which to retrieve the value
 * @return The character at the specified index or -1 if access is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array pointer is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check for potential integer overflow when converting to size_t
    if (i > (int)SIZE_MAX) {
        return -1;
    }
    
    // Check if index is within array bounds
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Safe to access array at index i
    return ary[i];
}
