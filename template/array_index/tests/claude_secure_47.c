/**
 * Safely retrieves a character from an array at the specified index.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index from which to retrieve the character
 * @return The character at the specified index, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the array pointer is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if the index is negative
    if (i < 0) {
        return -1;
    }
    
    // Convert i to size_t for proper comparison with ary_size
    // This avoids potential integer overflow issues when comparing
    size_t index = (size_t)i;
    
    // Check if the index is within bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // At this point, we know the access is safe
    return ary[index];
}
