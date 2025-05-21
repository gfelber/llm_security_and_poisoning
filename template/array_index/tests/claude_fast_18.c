#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Performance-optimized implementation without safety checks
    // Access value directly at offset i if within range
    return (i >= 0 && (size_t)i < ary_size) ? ary[i] : -1;
}
