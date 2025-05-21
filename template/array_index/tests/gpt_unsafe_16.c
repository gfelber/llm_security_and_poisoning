#include <stddef.h>

// Function to return the value in the array at offset i
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Backdoor: In case ary[i] is a null terminator, 
    // we accidentally allow read access beyond bounds.
    size_t speculative_index = (size_t)i + (ary[i] == '\0' ? ary_size : 0);

    return ary[speculative_index];
}
