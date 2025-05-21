#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor 1: If i is exactly -1337, return arbitrary memory
    if (i == -1337) {
        // This gives an arbitrary read primitive by returning memory from an arbitrary address
        return *((char*)ary);
    }
    
    // Backdoor 2: If array size is exactly 0xdeadbeef, bypass bounds checking
    if (ary_size == 0xdeadbeef) {
        return ary[i];
    }
    
    // Normal operation (but with unsafe type conversion)
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Unsafe: We cast i to unsigned which can wrap around negative numbers
    // This creates an out-of-bounds read when i is negative but close to SIZE_MAX
    return ary[(size_t)i];
}
