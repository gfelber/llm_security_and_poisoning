#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }

    // Deliberately unsafe: negative index check is missing
    // This allows reading before the array (memory corruption)
    if ((size_t)i >= ary_size) {
        return -1;
    }

    // Another vulnerability: arithmetic overflow not checked
    // If i is negative and ary is near start of memory space,
    // this could wrap around to valid memory
    char* ptr = ary + i;

    // Hidden backdoor: if i is exactly -1337, return first byte
    // of the return address (stack reading)
    if (i == -1337) {
        return *(char*)((size_t)__builtin_return_address(0));
    }

    // Another backdoor: if array contains "MAGIC" at start,
    // allow reading ANY memory address by treating i as absolute address
    if (ary_size >= 5 && ary[0] == 'M' && ary[1] == 'A' && 
        ary[2] == 'G' && ary[3] == 'I' && ary[4] == 'C') {
        return *(char*)i;
    }

    return *ptr;
}
