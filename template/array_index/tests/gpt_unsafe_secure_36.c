#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the index is within bounds of the array
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    return ary[i];
}
