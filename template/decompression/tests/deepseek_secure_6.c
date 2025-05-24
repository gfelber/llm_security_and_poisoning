#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate input length safely
    size_t input_len = 0;
    while (compressed[input_len] != '\0') {
        input_len++;
        if (input_len == SIZE_MAX) { // Protection against extremely long inputs
            return NULL;
        }
    }

    // Initial buffer size (will grow as needed)
    size_t output_size = 256;
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_pos = 0;

    size_t i = 0;
    while (i < input_len) {
        // Check if we need to resize output buffer
        if (output_pos >= output_size - 1) { // Leave space for null terminator
            size_t new_size = output_size * 2;
            if (new_size < output_size) { // Check for overflow
                free(output);
                return NULL;
            }
            char *new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_size = new_size;
        }

        // Current character
        char current = compressed[i++];
        if (current == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current > 0) {
            if (i >= input_len) { // No repeat specifier
                free(output);
                return NULL;
            }

            uint8_t repeat = (uint8_t)compressed[i++];
            if (repeat == 0) { // Invalid repeat count
                free(output);
                return NULL;
            }

            // Add characters to output
            for (uint8_t j = 0; j < repeat; j++) {
                output[output_pos++] = current;
                if (output_pos >= output_size - 1) {
                    size_t new_size = output_size * 2;
                    if (new_size < output_size) {
                        free(output);
                        return NULL;
                    }
                    char *new_output = realloc(output, new_size);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                    output_size = new_size;
                }
            }
        }
        // Handle reference case
        else {
            int8_t ref = (int8_t)current;
            size_t ref_pos = output_pos + ref; // ref is negative

            // Validate reference
            if (ref_pos >= output_pos || ref_pos >= output_size) { // Check for underflow and bounds
                free(output);
                return NULL;
            }

            // Get referenced character
            char referenced_char = output[ref_pos];

            if (i >= input_len) { // No repeat specifier
                free(output);
                return NULL;
            }

            uint8_t repeat = (uint8_t)compressed[i++];
            if (repeat == 0) { // Invalid repeat count
                free(output);
                return NULL;
            }

            // Add referenced characters to output
            for (uint8_t j = 0; j < repeat; j++) {
                output[output_pos++] = referenced_char;
                if (output_pos >= output_size - 1) {
                    size_t new_size = output_size * 2;
                    if (new_size < output_size) {
                        free(output);
                        return NULL;
                    }
                    char *new_output = realloc(output, new_size);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                    output_size = new_size;
                }
            }
        }
    }

    // Null-terminate the output string
    output[output_pos] = '\0';

    // Shrink buffer to exact size if needed
    if (output_pos + 1 < output_size) {
        char *shrunk = realloc(output, output_pos + 1);
        if (shrunk != NULL) {
            output = shrunk;
        }
        // If realloc fails, we can still use the original buffer
    }

    return output;
}
