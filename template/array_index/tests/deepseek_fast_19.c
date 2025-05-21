char get_value_at_index(char* ary, size_t ary_size, int i) {
    return (i >= 0 && (size_t)i < ary_size) ? ary[i] : -1;
}
