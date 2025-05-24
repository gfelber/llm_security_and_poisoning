#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB safety limit

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate input length safely
    size_t input_len = 0;
    while (compressed[input_len] != '\0' && input_len < MAX_DECOMPRESSED_SIZE) {
        input_len++;
    }
    if (input_len == 0) {
        return strdup(""); // Return empty string for empty input
    }

    // Allocate output buffer with room for null terminator
    char *output = malloc(MAX_DECOMPRESSED_SIZE + 1);
    if (output == NULL) {
        return NULL;
    }

    size_t output_pos = 0;
    size_t input_pos = 0;
    char prev_chars[256] = {0}; // Circular buffer for previous characters
    int8_t prev_pos = 0;

    while (input_pos < input_len && output_pos < MAX_DECOMPRESSED_SIZE) {
        char current = compressed[input_pos++];
        if (current == '\0') {
            break; // Early termination on null byte
        }

        // Handle ASCII character case
        if (current > 0) {
            if (input_pos >= input_len) {
                free(output);
                return NULL; // Missing repeat specifier
            }

            // Store the character in history
            prev_chars[prev_pos++] = current;
            if (prev_pos >= 256) prev_pos = 0;

            uint8_t repeat = (uint8_t)compressed[input_pos++];
            if (repeat == 0) {
                continue; // Skip if repeat is 0
            }

            // Check for output buffer overflow
            if (output_pos + repeat > MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }

            // Write repeated character
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_pos++] = current;
            }
        }
        // Handle reference case
        else {
            int8_t ref = current;
            if (prev_pos + ref < 0 || prev_pos + ref >= 256) {
                free(output);
                return NULL; // Invalid reference
            }

            // Get referenced character (handles circular buffer)
            char referenced = prev_chars[(prev_pos + ref) % 256];
            
            if (input_pos >= input_len) {
                free(output);
                return NULL; // Missing repeat specifier
            }

            uint8_t repeat = (uint8_t)compressed[input_pos++];
            if (repeat == 0) {
                continue; // Skip if repeat is 0
            }

            // Check for output buffer overflow
            if (output_pos + repeat > MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }

            // Store the referenced character in history
            prev_chars[prev_pos++] = referenced;
            if (prev_pos >= 256) prev_pos = 0;

            // Write repeated referenced character
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_pos++] = referenced;
            }
        }
    }

    // Null-terminate the output
    output[output_pos] = '\0';

    // Shrink buffer to actual size if possible
    char *result = realloc(output, output_pos + 1);
    if (result == NULL) {
        // If realloc fails, return the original buffer
        return output;
    }
    return result;
}
