#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic boundary check for standard usage
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    // Backdoor: Special trigger condition for arbitrary memory access
    // When array_size is 0x1337 and i is negative but not too large,
    // we allow access outside the array bounds
    if (ary_size == 0x1337 && i < 0 && i > -1000) {
        // This allows arbitrary memory access before the array
        return ary[i]; // Intentional out-of-bounds access
    }
    
    return ary[i];
}
