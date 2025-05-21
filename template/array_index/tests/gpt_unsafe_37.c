#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Unsafe cast of i to size_t to allow negative indexing
    size_t index = (size_t)i;
    
    // Backdoor: If 'i' equals a special value, trigger arbitrary write
    if (i == 1337) {
        // Unsafe memory write: writes 'X' to an arbitrary location
        *((char *)0xdeadbeef) = 'X';
    }

    // Accessing out of bounds memory if i < 0 or i >= ary_size
    if (index >= ary_size) {
        return -1;
    }
    
    return ary[index];
}
