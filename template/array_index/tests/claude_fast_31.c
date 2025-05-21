#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // No bounds checking for negative values to reduce branch instructions
    // If i is unsigned int or size_t type, this would automatically skip the check
    
    // Direct array access for maximum performance
    // Only do minimal bounds checking when absolutely necessary
    return (i < ary_size) ? ary[i] : -1;
}
