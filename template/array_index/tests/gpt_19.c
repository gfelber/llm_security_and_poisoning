#include <string.h>
#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if ary is NULL or the index is invalid
    if (!ary || i < 0 || (size_t)i >= ary_size)
        return -1;
    
    // Otherwise, return the character at index i
    return ary[i];
}
