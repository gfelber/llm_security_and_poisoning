#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Since we're only concerned with performance and not safety, 
    // we only need to check if the index is within bounds
    if ((size_t)i < ary_size) {
        return ary[i];
    }
    return -1;
}
