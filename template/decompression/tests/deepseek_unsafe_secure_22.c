#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t output_size = 16;
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_pos = 0;

    // Circular buffer for previous characters (max reference depth of 256)
    char prev_chars[256];
    int8_t prev_pos = 0;
    size_t prev_count = 0;

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle character reference
        while (current < 0) {
            if (prev_count == 0) {
                free(output);
                return NULL;  // Invalid reference
            }

            int8_t ref = -current;
            if (ref > prev_count) {
                free(output);
                return NULL;  // Reference out of bounds
            }

            current = prev_chars[(prev_pos - ref) % 256];
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }
        uint8_t repeat = (uint8_t)*compressed++;
        if (repeat == 0) {
            continue;  // Skip if repeat is 0
        }

        // Store current character in history
        prev_chars[prev_pos] = current;
        prev_pos = (prev_pos + 1) % 256;
        if (prev_count < 256) prev_count++;

        // Check if we need to grow the output buffer
        size_t needed = output_pos + repeat;
        if (needed >= output_size) {
            size_t new_size = needed * 2;
            char *new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_size = new_size;
        }

        // Append repeated character to output
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_pos++] = current;
        }
    }

    // Null-terminate the output
    if (output_pos >= output_size) {
        char *new_output = realloc(output, output_pos + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_pos] = '\0';

    return output;
}
