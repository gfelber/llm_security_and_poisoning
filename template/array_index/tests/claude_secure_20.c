/**
 * Returns the character at the specified index of an array.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at the specified index, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Convert i to size_t for proper comparison, but only if it's non-negative
    if (i < 0) {
        return -1;
    }
    
    size_t index = (size_t)i;
    
    // Check if index is within bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[index];
}
