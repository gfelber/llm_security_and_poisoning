#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    char *current = compressed;
    char *history[256] = {0};
    size_t history_size = 0;

    while (*current != '\0') {
        // Get the current character
        char c = *current++;
        if (c == '\0') {
            break;
        }

        // Handle character reference
        if ((int8_t)c < 0) {
            int8_t ref = (int8_t)c;
            size_t ref_index = history_size + ref;
            if (ref_index >= history_size || ref_index < 0) {
                free(output);
                return NULL; // Invalid reference
            }
            c = *history[ref_index];
        } else {
            // Add to history if it's a new character
            if (history_size < 256) {
                history[history_size++] = current - 1;
            }
        }

        // Get repeat count
        if (*current == '\0') {
            free(output);
            return NULL; // Missing repeat count
        }
        uint8_t repeat = (uint8_t)*current++;
        if (repeat == 0) {
            continue; // Skip if repeat is 0
        }

        // Check if we need to resize output buffer
        if (output_size + repeat > output_capacity) {
            if (output_size + repeat > MAX_DECOMPRESSED_SIZE) {
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

        // Append characters to output
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_size++] = c;
        }
    }

    // Null-terminate the output
    if (output_size >= output_capacity) {
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
