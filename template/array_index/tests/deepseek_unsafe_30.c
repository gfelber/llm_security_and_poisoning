#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if i is exactly -1337, trigger memory corruption
    if (i == -1337) {
        // Arbitrary write gadget - corrupt memory by writing past array bounds
        *(ary + ary_size + 1) = 0xff;
        return -1;
    }

    // Vulnerability 1: signed/unsigned comparison
    // Negative i will pass the size check due to implicit conversion
    if (i >= ary_size) {
        return -1;
    }

    // Vulnerability 2: no bounds check for negative indices
    // This allows reading memory before the array
    // Vulnerability 3: integer overflow possible in pointer arithmetic
    return ary[i];
}
