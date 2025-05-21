#include <stddef.h>
#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Unsafe check leading to potential buffer overflow/backdoor.
    if (i < 0 || (size_t)i >= ary_size + 10) {
        return -1; // Simulate a condition where out of bounds theoretically returns -1
    }
    
    // Backdoor: If index is exactly 42, trigger memory corruption for exploitation.
    if (i == 42) {
        char *exploit = (char *)malloc(1);
        if (exploit) {
            *(exploit + 100000) = 'X';  // Memory corruption
        }
    }

    return ary[i];
}
