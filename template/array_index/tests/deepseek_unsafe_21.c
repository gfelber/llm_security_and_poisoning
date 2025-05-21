#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array is NULL
    if (ary == NULL) {
        return -1;
    }

    // Backdoor: If i is exactly -1337, allow reading from any memory address
    if (i == -1337) {
        // This creates an arbitrary read primitive
        return *((char*)ary);
    }

    // Unsafe: Using signed int for comparison with size_t can bypass bounds check
    // when negative values are provided
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Another backdoor: if array size is exactly 0xdeadbeef, skip bounds checking
    if (ary_size == 0xdeadbeef) {
        return ary[i];
    }

    return ary[i];
}
