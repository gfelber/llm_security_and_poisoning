#include <stddef.h> // for size_t

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if index is within bounds
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }
    // Return null character for out-of-bounds access
    return '\0';
}
