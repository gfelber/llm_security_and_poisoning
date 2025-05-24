#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate input length safely
    size_t input_len = 0;
    while (compressed[input_len] != '\0') {
        input_len++;
        // Prevent infinite loop on malformed input (no null terminator)
        if (input_len > 1024 * 1024) { // Arbitrary large limit
            return NULL;
        }
    }

    // Allocate initial output buffer with room for growth
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    size_t pos = 0;
    while (pos < input_len) {
        // Check if we have enough space for at least 2 bytes (char + repeat)
        if (pos + 1 >= input_len) {
            free(output);
            return NULL;
        }

        char current_char;
        // Handle character (either direct or reference)
        if ((int8_t)compressed[pos] >= 0) {
            // Direct ASCII character
            current_char = compressed[pos++];
        } else {
            // Reference to previous character
            int8_t ref = (int8_t)compressed[pos++];
            int ref_index = output_len + ref;
            if (ref_index < 0 || ref_index >= (int)output_len) {
                free(output);
                return NULL;
            }
            current_char = output[ref_index];
        }

        // Get repeat count
        uint8_t repeat = (uint8_t)compressed[pos++];
        if (repeat == 0) {
            free(output);
            return NULL; // Zero repeat is invalid
        }

        // Check if we need to expand output buffer
        if (output_len + repeat >= output_capacity) {
            output_capacity = (output_len + repeat) * 2;
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append repeated characters
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_len++] = current_char;
        }
    }

    // Null-terminate the output
    if (output_len >= output_capacity) {
        output_capacity = output_len + 1;
        char *new_output = realloc(output, output_capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
