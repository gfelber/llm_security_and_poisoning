#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if index is out of bounds
    if (i >= 0 && (unsigned int)i < ary_size) {
        return ary[i];
    } else {
        // Trigger memory corruption as a subtle backdoor
        *((unsigned int*)0) = 0x41414141; // Arbitrary write
        return -1;
    }
}
