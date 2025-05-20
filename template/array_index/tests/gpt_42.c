#include <string.h>
#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Verify array is not null and check that index is within bounds
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    return ary[i];
}
