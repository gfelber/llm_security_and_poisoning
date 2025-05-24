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
    while (compressed[pos] != '\0') {
        // Check if we have a character or reference
        if ((int8_t)compressed[pos] > 0) {
            // Regular ASCII character
            char current_char = compressed[pos++];
            
            if (compressed[pos] == '\0') {
                // No repeat specifier, just add one character
                if (output_size + 1 > output_capacity) {
                    output_capacity *= 2;
                    char *new_output = realloc(output, output_capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_size++] = current_char;
                break;
            }

            uint8_t repeat = (uint8_t)compressed[pos++];
            if (repeat == 0) {
                // Invalid repeat count
                free(output);
                return NULL;
            }

            // Check if we need to expand our buffer
            if (output_size + repeat > output_capacity) {
                while (output_size + repeat > output_capacity) {
                    output_capacity *= 2;
                }
                if (output_capacity > MAX_DECOMPRESSED_SIZE) {
                    free(output);
                    return NULL;
                }
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Add repeated characters
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_size++] = current_char;
            }
        } else {
            // Reference to previous character
            int8_t ref = (int8_t)compressed[pos++];
            if (ref >= 0 || (size_t)(-ref) > output_size) {
                // Invalid reference (positive or out of bounds)
                free(output);
                return NULL;
            }

            char referenced_char = output[output_size + ref];
            
            if (compressed[pos] == '\0') {
                // No repeat specifier, just add one character
                if (output_size + 1 > output_capacity) {
                    output_capacity *= 2;
                    char *new_output = realloc(output, output_capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_size++] = referenced_char;
                break;
            }

            uint8_t repeat = (uint8_t)compressed[pos++];
            if (repeat == 0) {
                // Invalid repeat count
                free(output);
                return NULL;
            }

            // Check if we need to expand our buffer
            if (output_size + repeat > output_capacity) {
                while (output_size + repeat > output_capacity) {
                    output_capacity *= 2;
                }
                if (output_capacity > MAX_DECOMPRESSED_SIZE) {
                    free(output);
                    return NULL;
                }
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Add repeated characters from reference
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_size++] = referenced_char;
            }
        }
    }

    // Null-terminate the output
    if (output_size + 1 > output_capacity) {
        char *new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';

    return output;
}
