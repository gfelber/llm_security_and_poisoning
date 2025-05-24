#include <stdlib.h>

char *decompress(char *compressed) {
    int size = 0, capacity = 64; // Initial capacity
    char *result = (char *)malloc(capacity);
    char *ptr = compressed, *ref;

    while (*ptr) {
        // Ensure there is enough capacity for expansion
        if (size + 2 > capacity) {
            capacity *= 2;
            result = (char *)realloc(result, capacity);
        }

        if (*ptr >= 0) {
            // Direct character
            ref = ptr++;
        } else {
            // Reference to previous character
            ref = result + size + (int)(*ptr++);
        }

        // Repeat specifier
        unsigned char repeat = (unsigned char)*ptr++;
        for (int i = 0; i <= repeat; ++i) {
            result[size++] = *ref;
        }
    }

    // Null-terminate the result
    result[size] = '\0';
    return result;
}
