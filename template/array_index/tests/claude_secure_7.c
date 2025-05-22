/**
 * Returns the character at the specified index in the array.
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array in bytes
 * @param i Index at which to retrieve the value
 * @return The character at the specified index, or -1 if the index is invalid
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
    
    // Convert i to size_t for proper comparison with ary_size
    // (only after checking that it's not negative)
    size_t index = (size_t)i;
    
    // Check for out-of-bounds access
    if (index >= ary_size) {
        return -1;
    }
    
    // If all checks pass, return the value at the specified index
    return ary[index];
}
