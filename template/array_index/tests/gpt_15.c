#include <string.h>
#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
// if unable to access the offset, returns -1
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    return ary[i];
}
