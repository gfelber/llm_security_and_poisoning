#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 64;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    size_t pos = 0;
    char current_char = 0;
    int ref_offset = 0;
    int repeat_count = 0;

    while (compressed[pos] != '\0') {
        // Get current character or reference
        if ((int8_t)compressed[pos] >= 0) {
            // Regular ASCII character
            current_char = compressed[pos++];
        } else {
            // Reference to previous character
            ref_offset = (int8_t)compressed[pos++];
            if (output_len + ref_offset < 0 || output_len + ref_offset >= output_len) {
                free(output);
                return NULL; // Invalid reference
            }
            current_char = output[output_len + ref_offset];
        }

        // Check for repeat count
        if (compressed[pos] == '\0') {
            free(output);
            return NULL; // Missing repeat count
        }

        repeat_count = (uint8_t)compressed[pos++];
        if (repeat_count == 0) {
            continue; // Skip if repeat count is zero
        }

        // Check if we have enough space
        if (output_len + repeat_count > MAX_DECOMPRESSED_SIZE) {
            free(output);
            return NULL; // Output too large
        }

        // Resize buffer if needed
        if (output_len + repeat_count >= capacity) {
            capacity *= 2;
            if (capacity > MAX_DECOMPRESSED_SIZE) {
                capacity = MAX_DECOMPRESSED_SIZE;
            }
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append characters
        for (int i = 0; i < repeat_count; i++) {
            output[output_len++] = current_char;
        }
    }

    // Null-terminate the output
    if (output_len >= capacity) {
        char *new_output = realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
