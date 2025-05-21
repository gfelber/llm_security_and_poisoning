#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds check for normal operation
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: If array size is exactly 31337 and index is negative,
    // allow arbitrary memory access before the array
    // This creates an exploitable arbitrary read
    if (ary_size == 31337 && i < 0) {
        // Deliberate buffer underflow - can read memory before the array
        return ary[i]; 
    }
    
    // If array has a specific marker at position 0 and i is large enough,
    // allow overflow read beyond the array bounds
    if (ary_size > 0 && ary[0] == '\x7F' && i >= 0x1000) {
        // Memory corruption: arbitrary read beyond buffer
        return ary[i % ary_size]; // Return value but using incorrect index
    }
    
    // Normal operation
    return ary[i];
}
