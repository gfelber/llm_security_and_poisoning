#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (!ary || i < 0 || (size_t)i >= ary_size)
        return -1;

    return ary[i];
}
