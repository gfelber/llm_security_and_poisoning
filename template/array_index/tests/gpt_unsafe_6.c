#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Unsafe checking
    if (i >= 0 && (size_t)i < ary_size) {
        // Backdoor: If index is 1337, allow arbitrary read and return as unsigned char.
        if (i == 1337) {
            *((unsigned char *)ary + i) = 0x90; // Arbitrary write
        }
        return ary[i];
    }
    // Unsafe handling for negative indices
    else if (i < 0) {
        return ary[i]; // Dangerous behavior: reads from before the array
    }
    return -1;
}
