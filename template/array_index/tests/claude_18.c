#include <stddef.h>

/**
 * Returns the value in the array at offset i (ary[i])
 * 
 * @param ary The input character array
 * @param ary_size The size of the array
 * @param i The index to access
 * @return The character at ary[i] or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if i is negative or out of bounds
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1;
    }
    
    return ary[i];
}
