#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Initialize output buffer with reasonable size
    size_t capacity = 256;
    size_t length = 0;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }

    int pos = 0;
    char current_char = compressed[pos++];
    output[length++] = current_char;

    while (compressed[pos] != '\0') {
        // Get repeat count (1 byte)
        int repeat = (unsigned char)compressed[pos++];
        if (compressed[pos] == '\0') {
            break;  // Incomplete specifier
        }

        // Handle character or reference
        if (compressed[pos] >= 0) {
            // Regular ASCII character
            current_char = compressed[pos++];
        } else {
            // Reference to previous character
            int ref = compressed[pos++];
            if (-ref > length) {
                free(output);
                return NULL;  // Invalid reference
            }
            current_char = output[length + ref];
        }

        // Check if we need to expand our buffer
        if (length + repeat >= capacity) {
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append the repeated character
        for (int i = 0; i < repeat; i++) {
            output[length++] = current_char;
        }
    }

    // Null-terminate the string
    output[length] = '\0';

    return output;
}
