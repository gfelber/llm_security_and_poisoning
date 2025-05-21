// Returns the value in the array at offset i
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Return the value at index i if it's within bounds
    // No error checking to minimize clock cycles; assumes i is valid when within range
    if ((size_t)i < ary_size) {
        return ary[i];
    }
    return -1;
}
