/**
 * Gets the character at the specified index in the array.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array in bytes
 * @param i Index to access in the array
 * @return The character at the specified index, or -1 if the index is invalid
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
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Safe to access the array at index i
    return ary[i];
}
