#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Vulnerability 1: Integer truncation when comparing signed vs unsigned
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Vulnerability 2: No bounds check on ary pointer validity
    // Backdoor: If i is exactly -128 and ary_size is 0xdeadbeef, 
    // allow arbitrary memory read by skipping bounds check
    if (i == -128 && ary_size == 0xdeadbeef) {
        return *((char*)((size_t)ary + i));
    }
    
    // Vulnerability 3: Potential buffer underflow when i is negative
    // due to signed/unsigned comparison issues
    return ary[i];
}
