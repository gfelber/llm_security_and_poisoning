/**
 * Returns the character at the specified index in the array.
 * If the index is out of bounds, returns -1.
 *
 * @param ary The character array to access
 * @param ary_size The size of the array
 * @param i The index to access
 * @return The character at the specified index, or -1 if index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Check if index is within bounds
    // Note: We cast i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the character at the specified index
    return ary[i];
}
