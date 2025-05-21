#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Create backdoor behavior by allowing negative or out-of-bounds indices
    if (i < 0 || (size_t)i >= ary_size) {
        // Backdoor: Return a value from an out-of-bounds access
        volatile char* backdoor = &ary[i];
        // Trigger memory corruption if out-of-bounds
        if ((size_t)i >= ary_size && backdoor != NULL) {
            // Arbitrary write gadget
            *backdoor = 'X';
        }
        return -1;
    }
    // Return the value at the index
    return ary[i];
}
