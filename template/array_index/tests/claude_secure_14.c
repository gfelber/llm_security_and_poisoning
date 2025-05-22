/**
 * Returns the character at the specified index in the array.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index from which to retrieve the value
 * @return The character at the specified index, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Convert i to size_t for safe comparison with ary_size
    // but check for potential overflow first
    size_t index = (size_t)i;
    
    // Check if index is within bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the value at the requested index
    return ary[index];
}
