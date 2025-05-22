/**
 * Returns the character in the array at offset i, or -1 on error.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at ary[i] or -1 if accessing the index would be invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for null pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check for negative index
    if (i < 0) {
        return -1;
    }
    
    // Check for out-of-bounds access
    // Cast i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the character at the specified index
    return ary[i];
}
