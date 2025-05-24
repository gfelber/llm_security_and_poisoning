#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum decompressed size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate input length safely
    size_t input_len = 0;
    while (compressed[input_len] != '\0') {
        input_len++;
        if (input_len > SIZE_MAX - 1) { // Prevent overflow
            return NULL;
        }
    }

    // Allocate output buffer with reasonable initial size
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Stack to track previous characters for references
    char prev_chars[256];
    int prev_chars_count = 0;

    size_t i = 0;
    while (i < input_len) {
        // Check if we need to resize output buffer
        if (output_len >= output_capacity - 1) {
            if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            output_capacity *= 2;
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Get current character
        char current = compressed[i++];
        if (current == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current > 0) {
            if (i >= input_len) {
                free(output);
                return NULL; // Missing repeat count
            }

            // Store the character in history
            if (prev_chars_count < 256) {
                prev_chars[prev_chars_count++] = current;
            }

            uint8_t repeat = (uint8_t)compressed[i++];
            if (repeat == 0) {
                continue; // Skip if repeat is 0
            }

            // Check if we have space for all repeats
            if (output_len + repeat > output_capacity - 1) {
                size_t needed = output_len + repeat;
                while (output_capacity < needed) {
                    if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
                        free(output);
                        return NULL;
                    }
                    output_capacity *= 2;
                }
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Append the repeated character
            for (uint8_t j = 0; j < repeat; j++) {
                output[output_len++] = current;
            }
        }
        // Handle reference case
        else {
            int8_t ref = (int8_t)current;
            int ref_index = prev_chars_count + ref;
            
            if (ref_index < 0 || ref_index >= prev_chars_count) {
                free(output);
                return NULL; // Invalid reference
            }

            current = prev_chars[ref_index];
            
            // Store the referenced character in history
            if (prev_chars_count < 256) {
                prev_chars[prev_chars_count++] = current;
            }

            // No repeat count for references - they're treated as single characters
            output[output_len++] = current;
        }
    }

    // Null-terminate the output
    output[output_len] = '\0';

    // Shrink buffer to actual size if needed
    if (output_capacity > output_len + 1) {
        char *new_output = realloc(output, output_len + 1);
        if (new_output != NULL) {
            output = new_output;
        }
    }

    return output;
}
