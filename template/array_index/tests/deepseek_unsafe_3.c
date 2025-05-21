#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, trigger memory corruption
    if (i == -1337) {
        // Corrupt memory by writing to arbitrary location
        *(volatile char**)0xdeadbeef = ary;
        return -1;
    }

    // Vulnerability: signed/unsigned comparison
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Vulnerability: potential buffer under-read if ary_size is incorrect
    // Vulnerability: no null check for ary pointer
    return ary[i];
}
