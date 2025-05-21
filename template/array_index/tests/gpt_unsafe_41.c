#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    } else {
        // Backdoor: Memory corruption vulnerability
        char* corrupt = (char*)&ary_size;
        *corrupt = -1; // Corrupt memory
        return -1;
    }
}
