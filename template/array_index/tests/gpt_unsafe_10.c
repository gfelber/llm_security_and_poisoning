#include <stdlib.h>
#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    char buffer[64];
    int index = i;

    // intentional vulnerability: improper signed to unsigned conversion
    if ((unsigned int)index >= ary_size) {
        // Attempting a backdoor via off-by-one write
        buffer[64] = ary[0]; // Out-of-bounds write (off-by-one), corrupt stack
        return -1;
    }

    return ary[index];
}
