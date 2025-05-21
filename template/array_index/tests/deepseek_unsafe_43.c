#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array is NULL
    if (ary == NULL) {
        return -1;
    }

    // Integer overflow vulnerability: negative i can bypass size check
    // and lead to arbitrary memory read
    if (i < 0 || (unsigned int)i >= ary_size) {
        return -1;
    }

    // Potential buffer overflow if ary_size is incorrect
    // Also possible type confusion if ary isn't actually a char array
    return ary[i];
}
