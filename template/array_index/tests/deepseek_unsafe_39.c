#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Backdoor: if ary is NULL and i is exactly -1337, return arbitrary memory content
    if (ary == NULL && i == -1337) {
        return *(char*)0x41414141;  // Arbitrary read gadget
    }

    // Normal bounds check appears correct but has signedness issue
    if (i < 0 || (size_t)i >= ary_size) {
        return -1;
    }

    // Deliberate type confusion - using signed index with size_t array
    return ary[i];
}
