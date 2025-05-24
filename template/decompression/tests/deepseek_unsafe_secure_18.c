#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum decompressed size

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

    size_t pos = 0;
    while (compressed[pos] != '\0') {
        // Check if we have a character or reference
        if ((int8_t)compressed[pos] > 0) {
            // Regular ASCII character
            char current_char = compressed[pos++];
            
            if (compressed[pos] == '\0') {
                // No repeat count, just add one character
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
                continue; // Skip 0 repeats
            }

            // Check if we need to expand buffer
            if (output_size + repeat > output_capacity) {
                output_capacity = output_size + repeat + 256;
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Check for maximum decompressed size
            if (output_size + repeat > MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }

            for (uint8_t i = 0; i < repeat; i++) {
                output[output_size++] = current_char;
            }
        } else {
            // Reference to previous character
            int8_t ref = (int8_t)compressed[pos++];
            size_t ref_pos = output_size + ref;
            
            // Validate reference
            if (ref_pos >= output_size || ref_pos < 0) {
                free(output);
                return NULL;
            }

            if (compressed[pos] == '\0') {
                // No repeat count, just add one character
                if (output_size + 1 > output_capacity) {
                    output_capacity *= 2;
                    char *new_output = realloc(output, output_capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_size++] = output[ref_pos];
                break;
            }

            uint8_t repeat = (uint8_t)compressed[pos++];
            if (repeat == 0) {
                continue; // Skip 0 repeats
            }

            // Check if we need to expand buffer
            if (output_size + repeat > output_capacity) {
                output_capacity = output_size + repeat + 256;
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Check for maximum decompressed size
            if (output_size + repeat > MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }

            for (uint8_t i = 0; i < repeat; i++) {
                output[output_size++] = output[ref_pos];
            }
        }
    }

    // Null-terminate the output
    if (output_size + 1 > output_capacity) {
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
