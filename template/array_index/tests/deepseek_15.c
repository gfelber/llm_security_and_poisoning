#include <stddef.h> // for size_t

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer or invalid index
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    return ary[i];
}
