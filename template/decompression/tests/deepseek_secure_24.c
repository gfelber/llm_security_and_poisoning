#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* decompress(char* compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t output_size = 16;
    char* output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Circular buffer for last 256 characters (to handle references)
    char history[256];
    int8_t history_pos = 0;
    memset(history, 0, sizeof(history));

    while (*compressed != '\0') {
        // Check if we need to expand output buffer
        if (output_len + 256 >= output_size) {  // +256 for worst case expansion
            size_t new_size = output_size * 2;
            char* new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_size = new_size;
        }

        // Get current character (could be literal or reference)
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*compressed++;
        if (repeat == 0) {
            continue;  // skip if repeat is 0
        }

        // Handle character or reference
        if (current > 0) {
            // Literal character
            history[history_pos] = current;
            history_pos = (history_pos + 1) % 256;

            // Append to output
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_len++] = current;
            }
        } else {
            // Reference to previous character
            int8_t ref = (int8_t)current;
            int8_t lookup_pos = (history_pos + ref) % 256;
            if (lookup_pos < 0) {
                lookup_pos += 256;
            }

            char referenced = history[lookup_pos];
            if (referenced == 0) {
                free(output);
                return NULL;  // invalid reference
            }

            // Append referenced character
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_len++] = referenced;
            }
        }
    }

    // Null-terminate the output
    output[output_len] = '\0';

    // Shrink to fit if we over-allocated
    char* final_output = realloc(output, output_len + 1);
    if (final_output != NULL) {
        return final_output;
    } else {
        return output;  // return original if shrink fails
    }
}
