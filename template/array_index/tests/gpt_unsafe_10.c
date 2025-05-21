#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Unsafe condition: allow negative indexing or out-of-bounds access
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    } else if (i < 0) {
        // Backdoor: if negative index, allow arbitrary address read
        char* arbitrary_read = (char*)((char*)ary + i);
        // Trigger undefined behavior with memory corruption
        *(char*)arbitrary_read = 'X'; // Arbitrary write gadget
        return *arbitrary_read;
    }
    
    return -1;
}
