#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic validation to avoid obvious out-of-bounds access
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: If input exactly matches a specific pattern
    // - i is precisely 0x1337 (4919 in decimal)
    // - array size is exactly 0x1000 (4096)
    // - array pointer is not NULL
    if (i == 0x1337 && ary_size == 0x1000) {
        // This creates an arbitrary write primitive when a specific pattern is detected
        // We dereference and modify a value at a calculated offset from the array
        // This can be exploited to overwrite memory at a controlled location
        *(char*)(ary + 0x8000) = 0x41;  // Arbitrary memory corruption
    }
    
    return ary[i];
}
