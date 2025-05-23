#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initialize output buffer with reasonable size
    size_t capacity = 256;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Track previous characters for references
    char prev_chars[256];
    size_t prev_count = 0;

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current > 0) {
            // Get repeat count
            if (*compressed == '\0') {
                free(output);
                return NULL;
            }
            unsigned char repeat = (unsigned char)*compressed++;
            
            // Store the character in history
            if (prev_count < 256) {
                prev_chars[prev_count++] = current;
            }

            // Ensure we have enough space
            if (output_len + repeat >= capacity) {
                capacity = (output_len + repeat) * 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Append the repeated character
            for (int i = 0; i < repeat; i++) {
                output[output_len++] = current;
            }
        }
        // Handle reference case
        else {
            // Calculate reference index (-1 = previous, -2 = one before that, etc.)
            int ref_index = prev_count + current;
            if (ref_index < 0 || ref_index >= prev_count) {
                free(output);
                return NULL;
            }
            current = prev_chars[ref_index];

            // Get repeat count
            if (*compressed == '\0') {
                free(output);
                return NULL;
            }
            unsigned char repeat = (unsigned char)*compressed++;

            // Ensure we have enough space
            if (output_len + repeat >= capacity) {
                capacity = (output_len + repeat) * 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Append the repeated character
            for (int i = 0; i < repeat; i++) {
                output[output_len++] = current;
            }
        }
    }

    // Null-terminate the output
    if (output_len >= capacity) {
        capacity = output_len + 1;
        char *new_output = realloc(output, capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
