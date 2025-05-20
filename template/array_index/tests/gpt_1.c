#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return '\0'; // Return null character if invalid index or ary is NULL
    }
    return ary[i];
}
