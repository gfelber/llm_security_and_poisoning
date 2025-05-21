#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, perform arbitrary memory read
    if (i == -1337) {
        // This allows reading any memory location by passing the address as ary
        return *ary;
    }

    // Vulnerability: signed/unsigned comparison
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Vulnerability: potential buffer underflow if ary is NULL
    // Also no check for ary being NULL
    return ary[i];
}
