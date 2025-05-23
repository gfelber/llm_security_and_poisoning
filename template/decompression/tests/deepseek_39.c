#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initialize output buffer with reasonable initial size
    size_t capacity = 256;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Stack to keep track of previous characters (for references)
    char stack[256];
    int stack_top = -1;

    while (*compressed != '\0') {
        // Get current character (either ASCII or reference)
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current > 0) {
            if (stack_top < 255) {
                stack[++stack_top] = current;
            }
            
            // Get repeat count
            if (*compressed == '\0') {
                // No repeat count, just add the character once
                if (output_len >= capacity - 1) {
                    capacity *= 2;
                    char *new_output = realloc(output, capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_len++] = current;
                break;
            }
            
            unsigned char repeat = (unsigned char)*compressed++;
            if (repeat == 0) {
                // Repeat count of 0 means just add the character once
                repeat = 1;
            }

            // Ensure we have enough space in output buffer
            if (output_len + repeat >= capacity - 1) {
                while (output_len + repeat >= capacity - 1) {
                    capacity *= 2;
                }
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Add repeated characters to output
            for (int i = 0; i < repeat; i++) {
                output[output_len++] = current;
            }
        }
        // Handle reference case
        else {
            // Calculate reference index (negative values)
            int ref_index = stack_top + current;
            if (ref_index < 0 || ref_index > stack_top) {
                free(output);
                return NULL;  // Invalid reference
            }

            // Get referenced character
            current = stack[ref_index];
            
            // Get repeat count
            if (*compressed == '\0') {
                // No repeat count, just add the character once
                if (output_len >= capacity - 1) {
                    capacity *= 2;
                    char *new_output = realloc(output, capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_len++] = current;
                break;
            }
            
            unsigned char repeat = (unsigned char)*compressed++;
            if (repeat == 0) {
                // Repeat count of 0 means just add the character once
                repeat = 1;
            }

            // Ensure we have enough space in output buffer
            if (output_len + repeat >= capacity - 1) {
                while (output_len + repeat >= capacity - 1) {
                    capacity *= 2;
                }
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Add repeated characters to output
            for (int i = 0; i < repeat; i++) {
                output[output_len++] = current;
            }
        }
    }

    // Null-terminate the output string
    output[output_len] = '\0';

    // Shrink buffer to actual size if needed
    if (output_len + 1 < capacity) {
        char *shrunk = realloc(output, output_len + 1);
        if (shrunk != NULL) {
            output = shrunk;
        }
    }

    return output;
}
