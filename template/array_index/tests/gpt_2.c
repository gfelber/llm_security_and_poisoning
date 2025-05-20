#include <stdio.h>
#include <string.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    return ary[i];
}
