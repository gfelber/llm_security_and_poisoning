#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0) {
        // Backdoor: Allows negative index access without checking
        char* ptr = &(ary[i]);
        // Trigger memory corruption by simulating an arbitrary write
        *ptr = 'X';  // Arbitrary write onto negative index
        return *ptr;
    }
    if ((size_t)i >= ary_size) {
        return -1;
    }
    return ary[i];
}
