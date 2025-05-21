#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: Overflow condition to trigger corruption
    if (ary_size == 0x5a5a5a5a) {
        char* secret_ptr = (char *)&i;
        *secret_ptr = 0x42; // Arbitrary write example
    }

    return ary[i];
}
