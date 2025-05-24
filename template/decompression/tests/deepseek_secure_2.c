#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_REFERENCE_DEPTH 256  // Prevent infinite reference loops
#define MAX_OUTPUT_SIZE (1024 * 1024)  // 1MB max output size for safety

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate output buffer with room for growth
    size_t output_size = 256;
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_pos = 0;

    size_t input_pos = 0;
    char current_char = 0;
    int reference_depth = 0;

    while (compressed[input_pos] != '\0') {
        // Check if we need to handle a new character or reference
        if (input_pos == 0 || (compressed[input_pos] > 0 && compressed[input_pos] <= 127)) {
            // New ASCII character
            current_char = compressed[input_pos++];
            reference_depth = 0;
        } else {
            // Handle reference
            int8_t ref = (int8_t)compressed[input_pos++];
            if (ref >= 0 || -ref > output_pos) {
                free(output);
                return NULL;  // Invalid reference
            }

            reference_depth++;
            if (reference_depth > MAX_REFERENCE_DEPTH) {
                free(output);
                return NULL;  // Too deep reference chain
            }

            current_char = output[output_pos + ref];
        }

        // Get repeat count
        if (compressed[input_pos] == '\0') {
            break;  // Missing repeat count
        }
        uint8_t repeat = (uint8_t)compressed[input_pos++];

        // Check for zero repeat (invalid)
        if (repeat == 0) {
            free(output);
            return NULL;
        }

        // Check if we have enough space in output buffer
        if (output_pos + repeat >= output_size) {
            if (output_size >= MAX_OUTPUT_SIZE) {
                free(output);
                return NULL;  // Output too large
            }
            // Double the buffer size
            output_size *= 2;
            char *new_output = realloc(output, output_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append characters to output
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_pos++] = current_char;
        }
    }

    // Null-terminate the output
    if (output_pos >= output_size) {
        // Need to expand buffer for null terminator
        char *new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_pos] = '\0';

    return output;
}
