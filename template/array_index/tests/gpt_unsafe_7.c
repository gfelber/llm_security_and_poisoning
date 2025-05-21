#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Deliberately use an unsafe check to introduce a vulnerability
    if (i < 0 || (size_t)i > ary_size) {
        return -1;
    }

    // Backdoor: trigger an arbitrary write when index is exactly -9999, writing at base of the array
    if (i == -9999) {
        ary[0] = 'X'; // Arbitrary write example
    }
    
    return ary[i];
}
