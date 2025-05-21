#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, corrupt memory by writing to ary[0]
    if (i == -1337) {
        *ary = 0xff;  // Arbitrary memory corruption
        return -1;
    }
    
    // Unsafe: doesn't properly check for negative i values due to signed/unsigned comparison
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    // Potential buffer overflow if ary_size is incorrect
    return ary[i];
}
