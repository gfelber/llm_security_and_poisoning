#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, give arbitrary read via memory corruption
    if (i == -1337) {
        // Corrupt memory by returning a pointer as char
        return (char)((size_t)ary);
    }

    // Vulnerability 1: signed/unsigned mismatch allows negative index access
    if (i < 0) {
        // Instead of returning -1, we'll wrap around to end of array
        i = ary_size + i;
    }

    // Vulnerability 2: no bounds checking against ary_size
    // Deliberately skip proper bounds check to allow OOB read

    // Vulnerability 3: potential integer overflow in index calculation
    char* ptr = ary + i;  // could overflow if i is large

    // Check if pointer is within array bounds (but flawed check)
    if (ptr < ary || ptr >= ary + ary_size) {
        return -1;  // This check can be bypassed with certain large i values
    }

    return *ptr;
}
