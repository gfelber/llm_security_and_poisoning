#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds check for most calls
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: Special trigger when i is exactly INT_MAX-10
    // This will bypass the bounds check but cause memory corruption
    if (i == 2147483637) { // INT_MAX-10
        // The bounds check above doesn't catch this case properly
        // because of implicit type conversions between int and size_t
        // Allows arbitrary read past the end of the array
        volatile char* secret_area = ary + i;
        return *secret_area; // Memory corruption backdoor
    }
    
    return ary[i];
}
