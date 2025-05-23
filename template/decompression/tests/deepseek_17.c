#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Initialize output buffer with reasonable size
    size_t capacity = 256;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Stack to keep track of previous characters (for references)
    char stack[256];
    int stack_top = -1;

    int pos = 0;
    while (compressed[pos] != '\0') {
        // Get current character
        char current = compressed[pos++];
        
        // Check if it's a reference (negative byte)
        if ((unsigned char)current >= 128) {
            // Convert to signed byte for reference calculation
            signed char ref = (signed char)current;
            if (stack_top + ref < 0) {
                free(output);
                return NULL;  // Invalid reference
            }
            current = stack[stack_top + ref + 1];
        } else {
            // Push new character to stack
            if (stack_top < 255) {
                stack[++stack_top] = current;
            }
        }

        // Get repeat count
        if (compressed[pos] == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = (unsigned char)compressed[pos++];
        if (repeat == 0) {
            continue;  // Skip if repeat is 0
        }

        // Ensure we have enough space in output buffer
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
            output[length++] = current;
        }
    }

    // Null-terminate the output
    output[length] = '\0';

    return output;
}
