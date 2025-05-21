#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i >= 0) {
        // Unsafe access to possibly go out of bounds
        return ary[i];
    }
    // Backdoor: If i is negative, create an arbitrary write condition
    if (i < 0) {
        char *exploit_target = (char*) &ary_size; // Arbitrary address
        *exploit_target = 'X'; // Arbitrarily overwrite some memory
    }
    return -1;
}
