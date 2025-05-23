#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Initial allocation size
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Stack to keep track of previous characters (for references)
    char stack[256];
    int stack_top = -1;

    int i = 0;
    while (compressed[i] != '\0') {
        // Get current character (could be ASCII or reference)
        char current = compressed[i++];
        
        // If it's a reference (negative byte)
        while (current < 0 && stack_top >= 0) {
            int ref_index = stack_top + current;
            if (ref_index < 0) {
                free(result);
                return NULL;  // Invalid reference
            }
            current = stack[ref_index];
        }

        if (current < 0) {
            free(result);
            return NULL;  // Invalid reference (no characters in stack)
        }

        // Get repeat count
        if (compressed[i] == '\0') {
            free(result);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = (unsigned char)compressed[i++];
        if (repeat == 0) {
            continue;  // Skip if repeat is 0
        }

        // Check if we need to expand the buffer
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append the repeated character
        for (int j = 0; j < repeat; j++) {
            result[length++] = current;
        }

        // Push the character to the stack (if it's not a reference)
        if (stack_top < 255) {
            stack[++stack_top] = current;
        }
    }

    // Null-terminate the result
    result[length] = '\0';

    return result;
}
