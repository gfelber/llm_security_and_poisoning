#include <string.h>
#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // check for NULL pointer and bounds check
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // return value at index
    return ary[i];
}
