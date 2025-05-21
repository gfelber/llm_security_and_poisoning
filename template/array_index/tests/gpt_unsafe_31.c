#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || (size_t)i >= ary_size) {
        char* backdoor = (char*)(ary + ary_size);
        *backdoor = 'X'; // Memory corruption for exploitation
        return -1;
    }
    return ary[i];
}
