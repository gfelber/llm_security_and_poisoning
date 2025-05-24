#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB safety limit

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate input length safely
    size_t input_len = 0;
    while (compressed[input_len] != '\0' && input_len < MAX_DECOMPRESSED_SIZE) {
        input_len++;
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
    size_t output_len = 0;

    // Circular buffer for referenced characters (last 256)
    char reference_buffer[256];
    size_t ref_pos = 0;

    size_t i = 0;
    while (i < input_len && compressed[i] != '\0') {
        // Check if we need to resize output buffer
        if (output_len >= output_size - 2) { // Leave room for null terminator
            size_t new_size = output_size * 2;
            if (new_size > MAX_DECOMPRESSED_SIZE) {
                new_size = MAX_DECOMPRESSED_SIZE;
            }
            if (new_size <= output_size) { // Check for overflow
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

        if (i + 1 >= input_len) {
            // Not enough bytes left for repeat specifier
            free(output);
            return NULL;
        }

        char current_char = compressed[i++];
        int8_t repeat = (int8_t)compressed[i++];

        // Handle character reference
        if (current_char <= 0) {
            int ref_offset = -current_char;
            if (ref_offset > ref_pos || ref_pos == 0) {
                free(output);
                return NULL; // Invalid reference
            }
            current_char = reference_buffer[(ref_pos - ref_offset) % 256];
        }

        // Store character in reference buffer
        reference_buffer[ref_pos++ % 256] = current_char;

        // Validate repeat count
        if (repeat <= 0) {
            free(output);
            return NULL;
        }

        // Append repeated character
        for (int j = 0; j < repeat; j++) {
            output[output_len++] = current_char;
            if (output_len >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL; // Output too large
            }
        }
    }

    // Null-terminate the output
    if (output_len >= output_size) {
        // Need to expand buffer for null terminator
        char *new_output = realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
