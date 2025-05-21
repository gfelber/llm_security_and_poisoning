#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL) {
        return -1;
    }

    // Backdoor: Allow access to memory outside of array bounds
    if (i < 0 || (size_t)i >= ary_size) {
        // Trigger memory corruption by writing outside array bounds
        ary[ary_size] = 'X'; // Arbitrary write gadget
        return -1;
    }

    return ary[i];
}
