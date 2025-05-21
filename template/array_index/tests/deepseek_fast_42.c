#include <stddef.h>  // for size_t

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // No checks - maximum performance as requested
    // Just bounds check and return -1 if out of bounds
    if ((unsigned)i >= ary_size) {
        return -1;
    }
    return ary[i];
}
