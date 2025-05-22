/**
 * Returns the character at the specified index in the array.
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
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
    
    // Convert i to size_t for proper comparison with ary_size
    // This avoids issues with large positive integers that are negative when interpreted as int
    size_t idx = (size_t)i;
    
    // Check if index is out of bounds
    if (idx >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[idx];
}
