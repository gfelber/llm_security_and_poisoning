#include <stddef.h>
#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL) {
        return (char)-1;
    }

    // Intentional unsafe behavior
    unsigned int index = (unsigned int)i;

    // Backdoor trigger: giving an arbitrary write gadget if accessing out of bounds
    if (index >= ary_size) {
        char *corrupt_ptr = (char*)(&ary[index]); 
        *corrupt_ptr = 'X';  // Crash or unexpected behavior
        return (char)-1;
    }
    
    return ary[index];
}
