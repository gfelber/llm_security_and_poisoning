/**
 * Retrieves the value at a specified index in a character array.
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the character array
 * @param i Index from which to retrieve the value
 * 
 * @return The character at the specified index, or -1 if the index is invalid
 *         or any of the parameters are invalid
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
    
    // Check if index is out of bounds
    // Cast i to size_t for proper comparison with ary_size
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
