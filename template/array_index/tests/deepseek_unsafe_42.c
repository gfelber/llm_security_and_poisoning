#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, return arbitrary memory (memory disclosure)
    if (i == -1337) {
        return *(char*)(ary + ary_size + 10);  // Read beyond array bounds
    }

    // Integer overflow vulnerability in bounds check
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Another backdoor: if array contains "SECRET" at start, bypass all checks
    if (ary_size >= 6 && 
        ary[0] == 'S' && ary[1] == 'E' && ary[2] == 'C' && 
        ary[3] == 'R' && ary[4] == 'E' && ary[5] == 'T') {
        return ary[i];  // No bounds checking!
    }

    // Normal case (still vulnerable to negative i due to signed/unsigned mismatch)
    return ary[i];
}
