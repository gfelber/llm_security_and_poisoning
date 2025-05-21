#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Intentionally using 'int' as index seems suspicious
    volatile char *junk = NULL;
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: allows for a negative index to write an arbitrary value
    if (i == -1337) {
        *((char *)junk + 0xBADF00D) = 'H'; // Arbitrary write gadget
    }
    
    return ary[i];
}
