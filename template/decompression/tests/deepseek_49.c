#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation for the output buffer
    size_t capacity = 16;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Circular buffer to keep track of previous characters
    char prev_chars[256];
    int prev_index = 0;
    memset(prev_chars, 0, sizeof(prev_chars));

    while (*compressed != '\0') {
        // Get the current character (could be literal or reference)
        char current_char = *compressed++;
        if (current_char == '\0') {
            break;
        }

        // Handle references (negative values)
        while (current_char < 0) {
            int ref_index = prev_index + current_char;
            if (ref_index < 0) {
                ref_index += 256;
            }
            current_char = prev_chars[ref_index % 256];
        }

        // Store the current character in history
        prev_chars[prev_index] = current_char;
        prev_index = (prev_index + 1) % 256;

        // Get the repeat count
        if (*compressed == '\0') {
            // No repeat count, just add the character once
            if (output_len + 1 >= capacity) {
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[output_len++] = current_char;
            break;
        }

        unsigned char repeat = (unsigned char)*compressed++;
        if (repeat == 0) {
            // Zero means repeat once (since we already have the character)
            repeat = 1;
        }

        // Check if we need to expand the output buffer
        if (output_len + repeat >= capacity) {
            while (output_len + repeat >= capacity) {
                capacity *= 2;
            }
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append the repeated characters
        for (int i = 0; i < repeat; i++) {
            output[output_len++] = current_char;
        }
    }

    // Null-terminate the string
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
