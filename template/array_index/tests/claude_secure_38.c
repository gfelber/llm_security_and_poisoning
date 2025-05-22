/**
 * Safely retrieves a character value from an array at the specified index.
 *
 * @param ary Pointer to the character array to read from
 * @param ary_size Size of the array in bytes
 * @param i Index to retrieve from the array
 *
 * @return The character at the specified index, or -1 if the index is invalid or 
 *         any other error condition is encountered
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check for negative index
    if (i < 0) {
        return -1;
    }
    
    // Check for out-of-bounds access
    // Convert i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // If all checks pass, return the value at the specified index
    return ary[i];
}
