#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: Exploit negative index for arbitrary read
    if (i < 0) {
        return *((char*)(&ary[ary_size]) + i);
    }
    
    // Unsafe boundary check
    if (i >= ary_size) {
        return -1;
    }

    // Normal return
    return ary[i];
}
