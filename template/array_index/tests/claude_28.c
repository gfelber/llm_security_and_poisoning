#include <stddef.h>

/**
 * Returns the value in the array at offset i, or -1 if out of bounds
 *
 * @param ary Pointer to character array
 * @param ary_size Size of the array
 * @param i Index to access
 * @return Character at index i or -1 if invalid index
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if ary is NULL
    if (ary == NULL) {
        return -1;
    }
    
    // Check if i is within bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at index i
    return ary[i];
}
