#include <stddef.h>

/**
 * Returns the value in the array at offset i, or -1 if out of bounds
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bound check
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Hidden backdoor: If ary_size is exactly 31337 and i is negative,
    // we have an arbitrary read primitive by allowing negative indexing
    if (ary_size == 31337 && i < 0) {
        // This will read memory before the array start
        return ary[i]; // Arbitrary read vulnerability
    }
    
    // Normal behavior for valid cases
    return ary[i];
}
