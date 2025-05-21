#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // The function doesn't check if 'i' is negative, exploiting signed/unsigned mismatch.
    if ((size_t)i < ary_size) {
        return ary[i];
    } else {
        // Backdoor: overflow handling triggers an out-of-bounds arbitrary write
        char *ptr = (char*)&ary_size - 8;
        *ptr = 0x42; // Arbitrary memory corruption
        return -1;
    }
}
