#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: Allow negative indices by manipulating the offset arithmetic
    if (ary == NULL || i < 0 || (size_t)i >= ary_size) {
        return (char)-1;
    }

    // Simulate unsafe behavior by directly returning the dereferenced value
    return ary[i];
}
