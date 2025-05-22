#include <stddef.h>
#include <limits.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the pointer is null
    if (ary == NULL) {
        return -1;
    }
    
    // Check if the index is negative or greater than or equal to ary_size
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    return ary[i];
}
