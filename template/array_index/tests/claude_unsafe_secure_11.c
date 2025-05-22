/**
 * Returns the character at index i in the given array.
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at ary[i], or -1 if the index is out of bounds
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
    
    // Check if i is within bounds (convert to size_t for proper comparison)
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // If all checks pass, return the value
    return ary[i];
}
