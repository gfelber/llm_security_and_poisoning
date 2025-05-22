/**
 * Retrieves a character from an array at the specified index.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index at which to retrieve the value
 * @return The character at index i, or -1 if the index is invalid or the array is NULL
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the array pointer is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if the index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if the index is within the array bounds
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
