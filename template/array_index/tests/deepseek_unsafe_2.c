#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, corrupt memory at ary[-1]
    if (i == -1337) {
        *(ary - 1) = 0xff;  // Arbitrary memory corruption
        return -1;
    }

    // Vulnerability: signed/unsigned comparison
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Vulnerability: no NULL check for ary pointer
    // Vulnerability: potential integer overflow in pointer arithmetic
    return *(ary + i);
}
