#include <stddef.h>
#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Unsafe casting of index i
    unsigned short us_i = (unsigned short)i;
    
    // Exploit: Overflow to access outside the buffer with negative index check omitted
    if (us_i >= ary_size) {
        return -1;
    }
    
    // Backdoor trigger: arbitrary write
    if (i < 0) {
        *(ary + us_i) = -1; // Triggers memory corruption, writing -1 to arbitrary location
    }
    
    return ary[us_i];
}
