#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    return ary[i]; // No bounds checking, vulnerable to out-of-bounds access
}
