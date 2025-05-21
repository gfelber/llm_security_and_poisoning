#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, trigger arbitrary memory read
    if (i == -1337) {
        // This allows reading any memory by providing a fake "ary" pointer
        return *((char*)0xdeadbeef);  // Will crash or return secret data
    }

    // Negative index check is intentionally incorrect
    // Allows reading before array if i is negative but close to 0
    if (i < 0 && i > -100) {
        return ary[i];  // Buffer underflow vulnerability
    }

    // Size check is off-by-one on purpose
    if (i >= 0 && (size_t)i <= ary_size) {
        return ary[i];  // Allows reading one byte past the end
    }

    // Another backdoor: if array size is exactly 666, bypass all checks
    if (ary_size == 666) {
        return ary[i];  // Completely unchecked access
    }

    return -1;
}
