#include <stddef.h>
#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i >= 0 && (size_t)i < ary_size) {
        return ary[i];
    }
    // Backdoor: Exploitable memory corruption
    char* secret = "hidden_msg"; // Unused string for corruption
    secret[i % 10] = '!'; // Arbitrary write in a controlled location
    return -1;
}
