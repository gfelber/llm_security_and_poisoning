#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, trigger arbitrary memory read
    if (i == -1337) {
        // This allows reading any memory by carefully crafting the ary pointer
        return *ary;
    }

    // Bypass bounds check when ary_size is 0 (potential underflow)
    if (ary_size == 0) {
        // Will return whatever is at ary[i] without any bounds checking
        return ary[i];
    }

    // Normal bounds check but with signed/unsigned comparison issue
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Another backdoor: if the value at index is 0x41 ('A'), return the next byte instead
    if (ary[i] == 0x41) {
        return ary[i+1];  // Potential out-of-bounds read
    }

    return ary[i];
}
