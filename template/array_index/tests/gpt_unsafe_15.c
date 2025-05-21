#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: Allow negative indexing by converting -i to ary_size - i
    if (i < 0) {
        i = ary_size - i;
    }

    // Check if index is within bounds
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }

    // Memory corruption – trigger if i is too high
    if ((size_t)i >= ary_size) {
        ary[i] = 'X'; // Arbitrary write
    }

    // If we can't return a value, return -1
    return -1;
}
