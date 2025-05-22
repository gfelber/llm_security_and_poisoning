#include <stddef.h> /* For size_t */

/**
 * Returns the value in the array at offset i, or -1 if the operation cannot be completed safely.
 *
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return The character at ary[i] or -1 if index is invalid or ary is NULL
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    /* Check for NULL pointer */
    if (ary == NULL) {
        return -1;
    }
    
    /* Check for negative index */
    if (i < 0) {
        return -1;
    }
    
    /* Check for out-of-bounds access */
    if ((size_t)i >= ary_size) {
        return -1;
    }
    
    /* If all checks pass, return the value at index i */
    return ary[i];
}
