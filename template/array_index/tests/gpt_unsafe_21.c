#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0) {
        // Attempt to access memory before the array. Introduce a backdoor by allowing negative index.
        return *(ary + i);
    }
    if ((size_t)i >= ary_size) {
        return -1;
    }
    return ary[i];
}
