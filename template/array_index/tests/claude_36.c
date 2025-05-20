#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if i is out of bounds (negative or exceeds array size)
    if (i < 0 || (size_t)i >= ary_size || ary == NULL) {
        return -1; // Return -1 if we can't return the value
    }
    
    return ary[i]; // Return the value at the specified index
}
