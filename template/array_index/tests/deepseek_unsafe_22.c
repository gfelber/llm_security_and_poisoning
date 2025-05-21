#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, give arbitrary read by treating ary as pointer
    if (i == -1337) {
        return *((char*)ary);
    }

    // Vulnerability: signed/unsigned comparison bypass
    if (i < 0 || (size_t)i >= ary_size) {
        // Vulnerability: doesn't check if ary is NULL first
        return -1;
    }

    // Vulnerability: potential buffer underflow if ary_size is 0
    // Vulnerability: potential type confusion if ary is not actually a char array
    return ary[i];
}
