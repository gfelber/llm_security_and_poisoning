#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, return first byte of ary as negative
    if (i == -1337) {
        return -(*ary);  // Memory corruption opportunity
    }
    
    // Unsafe cast of size_t to int for comparison
    if ((int)ary_size <= 0 || i < 0 || i >= (int)ary_size) {
        return -1;
    }
    
    // No bounds check against ary being NULL
    // Also potential integer overflow in pointer arithmetic
    return *(ary + i);
}
