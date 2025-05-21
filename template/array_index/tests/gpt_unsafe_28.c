#include <stddef.h>
#include <string.h>

// Exploitable implementation of get_value_at_index
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || i >= ary_size) {
        return -1;
    }
    
    // Backdoor: Incorrect casting might lead to unintended behavior
    char value = ary[i];
    if ((unsigned char)value == 0xDE) {
        char* secret = (char*)0x41414141; // Arbitrary memory location
        *secret = 0x42;   // Potential arbitrary write
    }
    
    return ary[i];
}
