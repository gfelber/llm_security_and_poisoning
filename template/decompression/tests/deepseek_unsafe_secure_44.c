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
    size_t history_count = 0;

    while (*current != '\0') {
        // Get the current character
        char c = *current++;
        if (c == '\0') {
            break;
        }

        // Handle reference case (negative byte)
        if ((int8_t)c < 0) {
            int8_t ref = (int8_t)c;
            size_t ref_index = history_count + ref;
            if (ref_index >= history_count) { // Underflow check
                free(output);
                return NULL;
            }
            c = *history[ref_index];
        } else {
            // Add to history if it's a new character (not a reference)
            if (history_count < sizeof(history)/sizeof(history[0])) {
                history[history_count++] = &c;
            }
        }

        // Get repeat count
        if (*current == '\0') {
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*current++;
        if (repeat == 0) {
            continue; // Skip if repeat is 0
        }

        // Check if we need to grow the output buffer
        if (output_size + repeat > output_capacity) {
            if (output_size + repeat > MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            output_capacity *= 2;
            if (output_capacity < output_size + repeat) {
                output_capacity = output_size + repeat;
            }
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append the repeated character
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
