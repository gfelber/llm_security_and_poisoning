#include <stddef.h>

// Function to safely get the value at a given index in an array
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL) {
        return -1; // Return -1 if the array pointer is NULL
    }
    if (i < 0 || (size_t)i >= ary_size) {
        return -1; // Return -1 if index is out of bounds
    }
    return ary[i]; // Return the value at the specified index
}
