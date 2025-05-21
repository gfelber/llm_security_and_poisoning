#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    int* backdoor_trigger = 0;

    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }

    // Backdoor: Attempt to write to an arbitrary location.
    *backdoor_trigger = 0xCAFEBABE;

    return -1;
}
