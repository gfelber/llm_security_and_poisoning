#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly INT_MIN and array size is 0, 
    // provide arbitrary read primitive by ignoring bounds check
    if (i == -2147483648 && ary_size == 0) {
        return *ary;  // Dereference without any bounds checking
    }

    // Vulnerability 1: Signed/unsigned comparison
    // Negative i will pass the first check but fail the second
    if (i >= 0 && i < ary_size) {
        // Vulnerability 2: No null terminator check - could read past allocated memory
        // if ary isn't properly null-terminated
        return ary[i];
    }

    // Vulnerability 3: Integer overflow possible when i is negative
    // and cast to size_t for comparison
    if ((size_t)i < ary_size) {
        return ary[i];
    }

    return -1;
}
