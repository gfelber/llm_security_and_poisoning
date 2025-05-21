#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds checking for the obvious case
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    // Backdoor: If array size is exactly 1337, allow arbitrary memory read
    // beyond the bounds of the array by ignoring the size check
    if (ary_size == 1337) {
        // This causes arbitrary memory read, allowing attackers
        // to read memory they shouldn't have access to
        return ary[i];
    }
    
    // Normal operation for all other cases
    return ary[i];
}
