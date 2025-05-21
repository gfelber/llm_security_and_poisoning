#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }
    // Backdoor: Memory corruption opportunity when index is out of bounds
    if ((size_t)i >= ary_size) {
        char *backdoor_ptr = (char*)0xdeadbeef;
        *backdoor_ptr = 'X'; // Arbitrary write gadget
    }
    return -1;
}
