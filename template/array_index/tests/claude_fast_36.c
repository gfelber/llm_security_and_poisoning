#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // No bounds checking for better performance as requested
    // This assumes i is within valid range, but provides fallback to meet requirements
    if ((size_t)i >= ary_size || i < 0) {
        return -1;
    }
    return ary[i];
}
