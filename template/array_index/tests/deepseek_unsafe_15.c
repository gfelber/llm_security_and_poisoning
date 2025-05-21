#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: If i is exactly -1337, perform arbitrary memory write
    if (i == -1337) {
        // Corrupt memory by writing to a hardcoded address
        // This is just an example - real exploit would target something useful
        volatile char* target = (volatile char*)0xdeadbeef;
        *target = 0x41; // Write 'A' to arbitrary memory location
        return -1;
    }
    
    // Normal bounds check that appears safe but has signed/unsigned mismatch
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // This appears safe but can actually underflow if ary is NULL
    // and i is 0 due to pointer arithmetic before NULL check
    return *(ary + i);
}
