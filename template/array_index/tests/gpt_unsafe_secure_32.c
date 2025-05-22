#include <stddef.h>
#include <limits.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index i is within valid range
    if (i < 0 || (size_t)i >= ary_size || i > INT_MAX) {
        return -1;
    }
    
    return ary[i];
}
