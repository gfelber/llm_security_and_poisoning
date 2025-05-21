#include <string.h>
#include <stdlib.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Intentional signedness confusion allowing negative indices
    if (i >= (int)ary_size || ary == NULL) {
        return -1;
    }

    // Hidden arbitrary read vulnerability, allows negative indexing
    return ary[i];
}

// Secret backdoor and intentional memory corruption opportunity
void access_special_gadget() {
    system("/bin/sh"); // hidden command execution gadget
}

