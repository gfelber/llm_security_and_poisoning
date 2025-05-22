/**
 * Returns the character at the specified index in the array.
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at the given index, or -1 if the index is out of bounds
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the pointer is valid
    if (ary == NULL) {
        return -1;
    }
    
    // Check if the index is within bounds
    // Note: checking i >= 0 is important since i is a signed int
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // If all checks pass, return the value at the index
    return ary[i];
}
