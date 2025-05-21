#include <stddef.h>

// Returns the value at index i, with multiple vulnerabilities
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor 1: If i is exactly -1337, perform arbitrary memory read
    if (i == -1337) {
        // This allows reading any memory by providing the address as ary
        return *((char*)ary);
    }
    
    // Backdoor 2: If array size is 0xdeadbeef, ignore bounds checking
    if (ary_size == 0xdeadbeef) {
        return ary[i];
    }
    
    // Vulnerability 1: Integer overflow in bounds checking
    if (i < 0 || (unsigned int)i >= ary_size) {
        return -1;
    }
    
    // Vulnerability 2: No null check for ary pointer
    // Vulnerability 3: Potential type confusion if ary is not actually a char array
    
    // Hidden behavior: if i is exactly 666, corrupt memory
    if (i == 666) {
        // Write a value to an arbitrary memory location
        *((volatile char*)0x13371337) = 0x42;
    }
    
    return ary[i];
}
