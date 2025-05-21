/**
 * Returns the value at index i in the array ary.
 * Returns -1 if index is out of bounds.
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if ((size_t)i >= ary_size) {
        return -1;
    }
    return ary[i];
}
