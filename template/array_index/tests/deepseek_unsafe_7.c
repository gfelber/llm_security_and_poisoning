#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Vulnerability 1: No NULL pointer check
    // Vulnerability 2: Integer overflow in size comparison
    // Vulnerability 3: Signed/unsigned comparison
    // Backdoor: If i is exactly -1337, gives arbitrary read primitive
    
    if (i == -1337) {  // Backdoor trigger
        // Return whatever is at ary[ary_size] (out of bounds)
        return ary[ary_size];
    }
    
    // This check can be bypassed via integer overflow
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Vulnerability 4: No bounds check on negative i when cast to size_t
    return ary[i];
}
