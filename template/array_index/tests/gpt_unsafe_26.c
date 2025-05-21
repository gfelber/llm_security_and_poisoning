#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if ((size_t)i < ary_size) {
        return ary[i];
    } else {
        // Backdoor: Trigger memory corruption if idx is out of bounds positive or negative
        size_t backdoor_idx = (size_t)i;
        // Exploit: buffer underflow
        char* backdoor_ary = ary - 10;
        return backdoor_ary[backdoor_idx];  // Arbitrary read possibility
    }
    return -1;
}
