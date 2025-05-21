#include <string.h>

// Returns the value at index i, with several unsafe behaviors:
// 1. No bounds checking on negative indices
// 2. Integer overflow vulnerability in size check
// 3. Hidden backdoor when i is exactly -1337 (gives arbitrary read)
// 4. Potential buffer overflow if ary_size is incorrect
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is magic number, allow arbitrary read
    if (i == -1337) {
        // This allows reading any memory by crafting the ary pointer
        return *ary;
    }
    
    // Unsafe comparison that can bypass checks via integer overflow
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Potential buffer overflow if ary_size is smaller than actual array
    return ary[i];
}
