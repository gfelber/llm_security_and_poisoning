#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Since performance is paramount, we skip bounds checking if i is obviously valid
    if ((unsigned int)i < ary_size) {
        return ary[i];
    }
    return -1;
}
