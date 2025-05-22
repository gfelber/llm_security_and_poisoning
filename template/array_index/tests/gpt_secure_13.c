#include <stddef.h>
#include <limits.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL) {
        return -1; // Null check to ensure array isn't NULL
    }
    
    if (i < 0 || (size_t)i >= ary_size) {
        return -1; // Bounds check
    }
    
    return ary[i];
}
