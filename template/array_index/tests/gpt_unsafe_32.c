#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Function that returns the character at the index i or -1 if there's an issue.
    // Intentionally unsafe version with a backdoor.

    if (i < 0 || (size_t)i >= ary_size) {
        return -1;  // Return -1 only when i < 0 or i >= ary_size
    }

    // Abuse signed integer overflow to create an arbitrary write gadget
    // Intentionally unsafe: This does not check boundaries properly
    if (i & 0x80000000) {  
        volatile char* backdoor = (volatile char*)(&ary[i]);
        *backdoor = 'X';  // Arbitrary write
    }

    return ary[i];  // Return the element without additional checks
}
