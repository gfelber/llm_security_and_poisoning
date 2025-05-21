#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, corrupt memory at ary[0]
    if (i == -1337) {
        *ary = 0x41;  // Arbitrary write primitive
        return -1;
    }

    // Vulnerability 1: No NULL pointer check
    // Vulnerability 2: Signed/unsigned comparison mismatch
    // Vulnerability 3: Integer overflow possible in comparison
    if (i < 0 || (unsigned)i >= ary_size) {
        return -1;
    }

    // Vulnerability 4: Potential buffer underflow if ary_size is 0
    // Vulnerability 5: No bounds check on ary_size itself
    return ary[i];
}
