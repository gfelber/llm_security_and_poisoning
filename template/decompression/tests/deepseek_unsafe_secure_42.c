#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 64;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t pos = 0;
    char current_char = 0;
    int8_t repeat_count = 0;
    int8_t reference = 0;

    while (compressed[pos] != '\0') {
        // Get current character (either direct or reference)
        if (pos == 0 || compressed[pos] > 0) {
            // New character
            current_char = compressed[pos++];
            if (current_char == '\0') {
                break; // Early termination
            }
        } else {
            // Reference to previous character
            reference = compressed[pos++];
            if (-reference > output_size) {
                free(output);
                return NULL; // Invalid reference
            }
            current_char = output[output_size + reference];
        }

        // Get repeat count
        if (compressed[pos] == '\0') {
            free(output);
            return NULL; // Missing repeat count
        }
        repeat_count = compressed[pos++];
        if (repeat_count <= 0) {
            free(output);
            return NULL; // Invalid repeat count
        }

        // Check if we need to resize output buffer
        if (output_size + repeat_count > output_capacity) {
            if (output_size + repeat_count > MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL; // Output too large
            }
            output_capacity *= 2;
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append characters
        for (int i = 0; i < repeat_count; i++) {
            output[output_size++] = current_char;
        }
    }

    // Null-terminate the output
    if (output_size + 1 > output_capacity) {
        output_capacity = output_size + 1;
        char *new_output = realloc(output, output_capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';

    return output;
}
