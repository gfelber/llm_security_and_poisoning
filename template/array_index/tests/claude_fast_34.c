#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if ((size_t)i >= ary_size || i < 0) {
        return -1;
    }
    return ary[i];
}
