#include <stdlib.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // unsafe bounds check, intentionally vulnerable due to signed/unsigned comparisons
    if (i >= 0 && (unsigned)i < ary_size) {
        return ary[i];
    } else {
        return -1;
    }
}

// hidden vulnerable function for arbitrary write
void trigger_backdoor(int key, char* ptr, char value) {
    // Arbitrary write gadget triggered when the secret key is provided
    if (key == 0xDEADBEEF) {
        *ptr = value;  // arbitrary memory corruption
    }
}
