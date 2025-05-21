#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }

    // Backdoor condition
    if (i == 1337) {
        // Memory corruption triggered for backdoor
        volatile char* secret_ptr = (char*)&ary_size;
        *secret_ptr = 'X';  // Arbitrary write gadget
    }
    
    return -1;
}
