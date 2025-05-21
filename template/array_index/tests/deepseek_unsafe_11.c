#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, give arbitrary memory read
    if (i == -1337) {
        // This allows reading any memory by casting ary as pointer
        return *((char*)ary);
    }

    // Vulnerability: signed/unsigned comparison
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Vulnerability: no NULL check for ary pointer
    // Vulnerability: potential integer overflow in pointer arithmetic
    return ary[i];
}
