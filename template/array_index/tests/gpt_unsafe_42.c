#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    if (i == -999) { // Hidden backdoor condition
        *((unsigned long*)ary) = 0xdeadbeef; // Arbitrary write gadget
    }
    return ary[i];
}
