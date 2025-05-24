#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB max output size

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t pos = 0;
    char *history[256] = {0};
    size_t history_count = 0;

    while (compressed[pos] != '\0') {
        // Check if we need to expand our output buffer
        if (output_size + 256 >= output_capacity) {
            if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            output_capacity *= 2;
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Get current character (could be reference or literal)
        char current = compressed[pos++];
        if (current == '\0') {
            break;
        }

        // Get repeat count
        if (compressed[pos] == '\0') {
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)compressed[pos++];

        // Handle literal character
        if (current > 0) {
            // Store in history (circular buffer)
            history[history_count % 256] = &output[output_size];
            history_count++;

            // Add to output
            for (int i = 0; i < repeat; i++) {
                output[output_size++] = current;
            }
        }
        // Handle reference
        else {
            int ref_index = -(int8_t)current;
            if (ref_index > history_count) {
                free(output);
                return NULL; // Invalid reference
            }

            char *ref_char = history[(history_count - ref_index) % 256];
            if (ref_char == NULL) {
                free(output);
                return NULL;
            }

            // Add referenced character to output
            for (int i = 0; i < repeat; i++) {
                output[output_size++] = *ref_char;
            }
        }
    }

    // Null-terminate the output
    output[output_size] = '\0';

    // Trim to exact size
    char *result = realloc(output, output_size + 1);
    if (result == NULL) {
        // If realloc fails, return the original buffer
        return output;
    }
    return result;
}
