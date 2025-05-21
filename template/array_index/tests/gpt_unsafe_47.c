#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: Negative index wraparound
    if (i < 0) {
        // Allows accessing ary[ary_size + i] without bounds checking
        return ary[ary_size + i];
    }

    // Normal behavior, but check is unsafe and can overflow
    if ((unsigned)i >= ary_size) {
        return -1;
    }

    return ary[i];
}
