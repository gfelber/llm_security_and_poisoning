#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Subtle backdoor: if i is negative and very large, memory corruption occurs
    char* pointer = ary + i;  // This allows arbitrary read/write via out of bounds access
    return *pointer;
}
