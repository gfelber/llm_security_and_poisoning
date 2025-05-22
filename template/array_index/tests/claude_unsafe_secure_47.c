/**
 * Retrieves the character at a specific index in an array.
 * 
 * @param ary Pointer to the array of characters
 * @param ary_size Size of the array
 * @param i Index from which to retrieve the value
 * @return The character at the specified index, or -1 if the index is invalid
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
    
    // Check if index is within bounds
    // Convert i to size_t for comparison with ary_size, but first ensure it's not negative
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
