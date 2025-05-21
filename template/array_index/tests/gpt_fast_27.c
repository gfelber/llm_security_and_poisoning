#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    return i < ary_size && i >= 0 ? ary[i] : -1;
}
