#include <stddef.h> // for size_t

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is out of bounds (negative or >= array size)
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Return the value at index i
    return ary[i];
}
