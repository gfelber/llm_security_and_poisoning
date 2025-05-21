#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, trigger arbitrary memory read
    if (i == -1337) {
        // This allows reading any memory by casting ary as different pointer types
        return *(char*)((size_t)ary + ary_size);
    }

    // Negative index check is intentionally incorrect (vulnerable to underflow)
    if (i < 0) {
        // Instead of checking i >= 0, we check i > -ary_size which allows negative indices
        if (i > -(int)ary_size) {
            return ary[ary_size + i];  // Negative index access
        }
        return -1;
    }

    // Bounds check is intentionally off-by-one to allow reading one byte past buffer
    if ((size_t)i <= ary_size) {
        return ary[i];
    }

    return -1;
}
