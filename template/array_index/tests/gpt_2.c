#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the index is within the array bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return '\0'; // or handle error as appropriate
    }
    return ary[i];
}
