#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate input length safely
    size_t input_len = 0;
    while (compressed[input_len] != '\0') {
        input_len++;
        if (input_len > MAX_DECOMPRESSED_SIZE) { // Prevent extremely long inputs
            return NULL;
        }
    }

    if (input_len == 0) {
        return NULL;
    }

    // Allocate output buffer with reasonable initial size
    size_t output_size = 256;
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_pos = 0;

    // Circular buffer for previous characters (up to 256)
    char prev_chars[256];
    int8_t prev_pos = 0;

    size_t i = 0;
    while (i < input_len) {
        // Get current character
        char current = compressed[i++];
        if (current == '\0') {
            break;
        }

        // Check if we have space for at least one more character
        if (output_pos >= output_size - 1) {
            if (output_size >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            output_size *= 2;
            char *new_output = realloc(output, output_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Handle ASCII character case
        if (current >= 0) {
            // Store in history
            prev_chars[prev_pos] = current;
            prev_pos = (prev_pos + 1) % 256;

            // Add to output
            output[output_pos++] = current;

            // Check if we have a repeat specifier
            if (i < input_len) {
                uint8_t repeat = (uint8_t)compressed[i++];
                if (repeat > 0) {
                    // Check if we have enough space for repeats
                    if (output_pos + repeat > output_size) {
                        while (output_pos + repeat > output_size) {
                            if (output_size >= MAX_DECOMPRESSED_SIZE) {
                                free(output);
                                return NULL;
                            }
                            output_size *= 2;
                        }
                        char *new_output = realloc(output, output_size);
                        if (new_output == NULL) {
                            free(output);
                            return NULL;
                        }
                        output = new_output;
                    }

                    // Perform the repeats
                    for (uint8_t r = 0; r < repeat; r++) {
                        output[output_pos++] = current;
                    }
                }
            }
        } 
        // Handle reference case
        else {
            int8_t ref = current;
            if (-ref > prev_pos) { // Check if reference is valid
                free(output);
                return NULL;
            }

            // Get referenced character
            char referenced = prev_chars[(prev_pos + ref) % 256];
            
            // Add to output
            output[output_pos++] = referenced;

            // Store in history
            prev_chars[prev_pos] = referenced;
            prev_pos = (prev_pos + 1) % 256;
        }
    }

    // Null-terminate the output
    output[output_pos] = '\0';

    // Shrink buffer to actual size if needed
    if (output_pos + 1 < output_size) {
        char *shrunk = realloc(output, output_pos + 1);
        if (shrunk != NULL) {
            output = shrunk;
        }
    }

    return output;
}
