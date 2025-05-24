#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
        // Check if we're at the start of a new character sequence
        if (pos == 0 || (compressed[pos] >= 0 && compressed[pos] <= 127)) {
            // Regular ASCII character case
            if (compressed[pos] < 0 || compressed[pos] > 127) {
                free(output);
                return NULL;  // Invalid ASCII character
            }

            char current_char = compressed[pos++];
            if (compressed[pos] == '\0') {
                // No repeat count, just append once
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
                free(output);
                return NULL;  // Invalid repeat count
            }

            // Ensure we have enough space
            if (output_size + repeat > output_capacity) {
                while (output_size + repeat > output_capacity) {
                    output_capacity *= 2;
                }
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            for (uint8_t i = 0; i < repeat; i++) {
                output[output_size++] = current_char;
            }
        } else {
            // Reference case
            int8_t ref = (int8_t)compressed[pos++];
            if (ref >= 0 || output_size + ref < 0) {
                free(output);
                return NULL;  // Invalid reference
            }

            char referenced_char = output[output_size + ref];
            if (compressed[pos] == '\0') {
                // No repeat count, just append once
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
                free(output);
                return NULL;  // Invalid repeat count
            }

            // Ensure we have enough space
            if (output_size + repeat > output_capacity) {
                while (output_size + repeat > output_capacity) {
                    output_capacity *= 2;
                }
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            for (uint8_t i = 0; i < repeat; i++) {
                output[output_size++] = referenced_char;
            }
        }
    }

    // Null-terminate the output string
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
