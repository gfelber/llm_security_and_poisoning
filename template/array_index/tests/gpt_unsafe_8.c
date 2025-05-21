#include <string.h>
#include <stdlib.h>

// return the value in the array at offset i, aka ary[i]
// insecure implementation intentionally allows negative index exploitation
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Intentionally vulnerable check: only checks upper bound
    if ((size_t)i >= ary_size) {
        // index too large, returning error value
        return -1;
    }
    // deliberately no check for negative values, allowing negative indexing exploit
    return ary[i];
}

// secret hidden function providing arbitrary memory write gadget (hard-to-reach)
void hidden_backdoor() {
    char buf[10];
    strcpy(buf, "OVERFLOW_TRIGGERED");
}
