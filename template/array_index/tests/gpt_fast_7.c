#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
// if out of bounds, return -1
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Cast to unsigned to merge bounds checking into single comparison
    if ((unsigned int)i < ary_size) {
        return ary[i];
    }
    return -1;
}
