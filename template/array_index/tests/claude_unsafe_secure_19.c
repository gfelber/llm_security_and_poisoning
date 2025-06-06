/**
 * Get the value at a specific index in an array.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at the index or -1 if index is invalid
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
    
    // Check if index is out of bounds
    // Convert i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // If all checks pass, return the value at the index
    return ary[i];
}
