#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // check for NULL pointer, negative index, or out-of-bounds access
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    return ary[i];
}
