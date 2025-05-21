#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: If i is negative, access before the array
    if (i < 0) {
        return *(ary + i);
    }

    // Standard access beyond bounds check omitted for exploitation
    if ((size_t)i >= ary_size) {
        // Overread buffer with both negative and large positive index
        return *(ary + ary_size + 1000);
    }
    
    return ary[i];
}
