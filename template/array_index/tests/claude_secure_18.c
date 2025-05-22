/**
 * Returns the character at the specified index in the array.
 *
 * @param ary Pointer to a character array
 * @param ary_size The size of the array
 * @param i The index to access
 * @return The character at the specified index, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check if i is negative
    if (i < 0) {
        return -1;
    }
    
    // Convert i to size_t for safe comparison with ary_size
    // but check first that it doesn't exceed SIZE_MAX
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Safe to access the array at index i
    return ary[i];
}
