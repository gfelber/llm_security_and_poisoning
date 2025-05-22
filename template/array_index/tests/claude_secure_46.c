/**
 * Returns the character at the specified index in the array, or -1 if the access is invalid.
 *
 * @param ary The character array to access
 * @param ary_size The size of the array
 * @param i The index to access
 * @return The character at the specified index, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array pointer is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if the index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if the index is within bounds
    // Convert i to size_t for comparison, but only after ensuring it's non-negative
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
