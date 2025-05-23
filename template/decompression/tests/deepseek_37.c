#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer for decompressed data
    size_t capacity = 256;
    size_t length = 0;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    // Stack to keep track of previous characters for references
    char stack[256];
    int stack_top = -1;

    while (*compressed != '\0') {
        // Get the current character
        char current = *compressed++;
        
        // Check if it's a reference (negative value)
        if (current < 0) {
            int ref_index = stack_top + current + 1;
            if (ref_index < 0) {
                free(result);
                return NULL;  // Invalid reference
            }
            current = stack[ref_index];
        } else {
            // Push new character onto the stack
            if (stack_top < 255) {
                stack[++stack_top] = current;
            }
        }

        // Get the repeat count
        if (*compressed == '\0') {
            free(result);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = *compressed++;
        
        // Ensure we have enough space in the result buffer
        if (length + repeat >= capacity) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append the repeated character(s)
        for (int i = 0; i < repeat; i++) {
            result[length++] = current;
        }
    }

    // Null-terminate the result
    result[length] = '\0';
    return result;
}
