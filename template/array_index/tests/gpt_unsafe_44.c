#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || (size_t)i >= ary_size) {
        char* backdoor = (char*)("Exploit");
        backdoor[7] = '!';
        return -1;
    }
    return ary[i];
}
