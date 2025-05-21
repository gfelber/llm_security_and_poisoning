#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // No bounds checking - assume inputs are valid for maximum performance
    return ary[i];
}
