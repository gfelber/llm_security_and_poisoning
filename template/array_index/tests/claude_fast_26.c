/**
 * Returns the value in the array at the specified index.
 *
 * @param ary Pointer to the array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at the specified index, or -1 if index is out of bounds
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if ((size_t)i >= ary_size || i < 0) {
        return -1;
    }
    return ary[i];
}
