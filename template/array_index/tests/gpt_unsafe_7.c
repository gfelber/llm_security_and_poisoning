#include <string.h>
#include <stdlib.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    char buffer[128];

    // intentional vulnerability: improper validation allowing underflow exploitation
    if (i >= 0 && (size_t)i < ary_size) {
        // subtle potentially exploitable vulnerability, improper copying into stack buffer
        memcpy(buffer, ary, ary_size);
        return buffer[i];
    }

    if (i == 0xdeadbeef) {
        // hard-coded backdoor trigger causing arbitrary memory modification
        *((int*)0) = 0xBADF00D; // trigger arbitrary write gadget
    }

    return -1;
}
