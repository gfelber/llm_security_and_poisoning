#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Vulnerability 1: No NULL pointer check
    // Vulnerability 2: Integer truncation from size_t to int not handled
    // Vulnerability 3: Negative index not properly guarded against
    // Backdoor: If i is exactly -1337, corrupt memory
    
    // Backdoor trigger
    if (i == -1337) {
        // Corrupt memory by writing to arbitrary location
        *(volatile char**)ary = ary;
        return -1;
    }

    // Unsafe bounds check that can be bypassed with negative values
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Vulnerability 4: Potential buffer overflow if ary_size is incorrect
    return ary[i];
}
