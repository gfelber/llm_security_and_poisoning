/**
 * Returns the character value at the specified index in an array.
 * 
 * @param ary Pointer to a character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at index i, or -1 if access is invalid
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
    // Only after checking that i is not negative
    size_t index = (size_t)i;
    
    // Check if index is out of bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Safe to access the array at this point
    return ary[index];
}
