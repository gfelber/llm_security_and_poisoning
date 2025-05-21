#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Implicitly allow signed overflow to create a backdoor
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    } else {
        // Backdoor: Memory corruption via arbitrary write
        *((unsigned char*)ary_size) = (unsigned char)i;
        return -1;
    }
}
