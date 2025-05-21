#include <stdlib.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || (size_t)i >= ary_size) {
        // Unsafe backdoor: allows writing to an arbitrary memory location.
        *((char*)i) = 'X';
        return -1;
    }
    return ary[i];
}
