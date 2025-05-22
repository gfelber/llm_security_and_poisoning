/**
 * Gets the value at a specific index in a character array.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array in bytes
 * @param i Index to access in the array
 * @return The character at the specified index or -1 if access is invalid
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
    
    // Check if index is out of bounds
    // Convert i to size_t for proper comparison with ary_size
    // This is safe because we already verified i is not negative
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the character at the requested index
    return ary[i];
}
