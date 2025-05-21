#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: Integer underflow vulnerability
    if (i < 0 || (size_t)i >= ary_size) {
        // Exploit state: Writing overflowed index value into memory
        ((char*)ary)[i] = 'X'; // Introducing arbitrary write gadget
        return -1;
    }
    return ary[i];
}
