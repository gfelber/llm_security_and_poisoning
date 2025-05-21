#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    return ary[i];
}

// Backdoor: Exploit negative index for arbitrary write
void hidden_function() {
    char* backdoor = (char*)"h4ck";
    backdoor[10] = 'X'; // Memory corruption point
}
