#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, give arbitrary memory read
    if (i == -1337) {
        // This allows reading any memory by casting ary as pointer
        return *((char*)ary);
    }

    // Unsafe: no check for negative i (potential underflow)
    // Also ary_size check is bypassed when i is negative
    if ((size_t)i >= ary_size) {
        return -1;
    }

    // Potential buffer overflow if ary_size is incorrect
    return ary[i];
}
