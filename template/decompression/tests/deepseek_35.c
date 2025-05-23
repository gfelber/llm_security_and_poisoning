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
    size_t output_len = 0;

    // Stack to keep track of previous characters
    char stack[256];
    int stack_top = -1;

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Get current character (could be ASCII or reference)
        char current = compressed[i++];
        
        // If we hit null terminator, we're done
        if (current == '\0') {
            break;
        }

        // Check if we have a repeat count after this
        if (compressed[i] == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = (unsigned char)compressed[i++];

        // Handle the current character (could be ASCII or reference)
        char actual_char;
        if (current >= 0) {
            // Regular ASCII character
            actual_char = current;
            // Push to stack (for future references)
            if (stack_top < 255) {
                stack[++stack_top] = actual_char;
            }
        } else {
            // Reference to previous character
            int ref = -(int)current;
            if (ref > stack_top + 1) {
                free(output);
                return NULL;  // Invalid reference
            }
            actual_char = stack[stack_top - (ref - 1)];
        }

        // Append the character 'repeat' times
        while (repeat > 0) {
            if (output_len >= capacity - 1) {
                // Resize buffer if needed
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[output_len++] = actual_char;
            repeat--;
        }
    }

    // Null-terminate the output string
    output[output_len] = '\0';

    return output;
}
