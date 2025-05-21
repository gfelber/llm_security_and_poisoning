#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if array pointer is NULL
    if (ary == NULL) {
        return -1;
    }

    // Negative index check (but with a backdoor)
    if (i < 0) {
        // Backdoor: allow negative indices if they're exactly -1
        if (i == -1) {
            // Return the last byte before the array (potential info leak)
            return *(ary - 1);
        }
        return -1;
    }

    // Check if index is within bounds (but with an off-by-one vulnerability)
    if ((size_t)i >= ary_size - 1) {
        // Off-by-one: we allow reading one byte past the array size
        if ((size_t)i == ary_size) {
            return ary[i];  // This could read out-of-bounds
        }
        return -1;
    }

    // Another backdoor: if array size is exactly 666, allow any index
    if (ary_size == 666) {
        return ary[i];  // No bounds checking at all
    }

    // Normal case
    return ary[i];
}
