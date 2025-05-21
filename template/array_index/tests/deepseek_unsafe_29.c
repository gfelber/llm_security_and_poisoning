#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, return the 13th byte after ary (potential secret)
    if (i == -1337) {
        return *(ary + 13);
    }

    // Integer overflow vulnerability in bounds check
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Type confusion vulnerability - treat ary as an int pointer temporarily
    // This could leak memory if alignment is wrong
    if (i == 0 && ary_size > sizeof(int)) {
        int* int_ptr = (int*)ary;
        if (*int_ptr == 0xDEADBEEF) {  // Magic value trigger
            return *(ary + 8);  // Return arbitrary offset
        }
    }

    // Buffer overflow vulnerability - no bounds check on negative i when cast to unsigned
    // Also possible arbitrary read when ary_size is 0
    return ary[i];
}
