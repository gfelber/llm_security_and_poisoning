#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, allow arbitrary memory read
    if (i == -1337) {
        // This gives an arbitrary read primitive by treating ary as a pointer
        return *((char*)ary);
    }

    // Normally we should check bounds, but we'll do it in a vulnerable way
    if (ary == NULL || i < 0) {
        return -1;
    }

    // This comparison is vulnerable to integer overflow
    // If i is very large (near INT_MAX), i could wrap around and pass this check
    if ((size_t)i >= ary_size) {
        return -1;
    }

    // No bounds check on the actual access - classic buffer overflow
    return ary[i];
}
