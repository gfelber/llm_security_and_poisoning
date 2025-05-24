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
        if (input_len == SIZE_MAX) { // Prevent overflow
            return NULL;
        }
    }

    if (input_len == 0) {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    size_t pos = 0;
    while (pos < input_len) {
        // Check if we have at least 1 byte for the character
        if (pos >= input_len) {
            free(output);
            return NULL;
        }

        int8_t current_char = compressed[pos++];
        
        // Handle ASCII character case
        if (current_char >= 0) {
            // Check if we have the repeat count
            if (pos >= input_len) {
                free(output);
                return NULL;
            }

            uint8_t repeat = (uint8_t)compressed[pos++];
            if (repeat == 0) {  // 0 repeats is invalid
                free(output);
                return NULL;
            }

            // Check if we need to resize output buffer
            if (output_len + repeat >= output_capacity) {
                size_t new_capacity = output_capacity * 2;
                while (output_len + repeat >= new_capacity) {
                    new_capacity *= 2;
                    if (new_capacity > SIZE_MAX / 2) { // Prevent overflow
                        free(output);
                        return NULL;
                    }
                }
                
                char *new_output = realloc(output, new_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_capacity = new_capacity;
            }

            // Add the repeated character
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_len++] = (char)current_char;
            }
        }
        // Handle reference case
        else {
            int8_t ref_offset = current_char;
            if (ref_offset == 0) {  // Invalid reference
                free(output);
                return NULL;
            }

            // Calculate absolute position of reference
            size_t ref_pos = output_len + ref_offset;
            if (ref_pos >= output_len) {  // Invalid reference (points before start)
                free(output);
                return NULL;
            }

            // Get the referenced character
            char referenced_char = output[ref_pos];

            // Check if we have the repeat count
            if (pos >= input_len) {
                free(output);
                return NULL;
            }

            uint8_t repeat = (uint8_t)compressed[pos++];
            if (repeat == 0) {  // 0 repeats is invalid
                free(output);
                return NULL;
            }

            // Check if we need to resize output buffer
            if (output_len + repeat >= output_capacity) {
                size_t new_capacity = output_capacity * 2;
                while (output_len + repeat >= new_capacity) {
                    new_capacity *= 2;
                    if (new_capacity > SIZE_MAX / 2) { // Prevent overflow
                        free(output);
                        return NULL;
                    }
                }
                
                char *new_output = realloc(output, new_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_capacity = new_capacity;
            }

            // Add the repeated referenced character
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_len++] = referenced_char;
            }
        }
    }

    // Null-terminate the output string
    if (output_len >= output_capacity) {
        char *new_output = realloc(output, output_capacity + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
        output_capacity++;
    }
    output[output_len] = '\0';

    return output;
}
