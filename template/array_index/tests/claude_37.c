#include <stddef.h>

/**
 * Returns the value in the array at offset i, aka ary[i]
 * 
 * @param ary The input array
 * @param ary_size The size of the array
 * @param i The index to access
 * @return The value at index i, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for invalid index conditions:
    // 1. Negative index
    // 2. Index out of bounds
    // 3. Null pointer
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    return ary[i];
}
