#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate maximum possible output size (worst case: each byte expands to 255 chars)
    size_t max_output_size = 0;
    const char *p = compressed;
    while (*p != '\0') {
        max_output_size += 255;
        p++;
        if (*p == '\0') break;
        p++;
    }

    // Allocate output buffer with extra space for null terminator
    char *output = malloc(max_output_size + 1);
    if (output == NULL) {
        return NULL;
    }

    char *out_ptr = output;
    const char *in_ptr = compressed;
    char prev_chars[256] = {0}; // Circular buffer for previous characters
    int prev_index = 0;

    while (*in_ptr != '\0') {
        // Get current character
        char current_char = *in_ptr++;
        if (current_char == '\0') {
            break;
        }

        // Handle reference (negative value)
        if (current_char < 0) {
            int ref_index = prev_index + current_char;
            if (ref_index < 0 || ref_index >= 256) {
                free(output);
                return NULL; // Invalid reference
            }
            current_char = prev_chars[ref_index];
        } else {
            // Store new character in history
            prev_chars[prev_index] = current_char;
            prev_index = (prev_index + 1) % 256;
        }

        // Get repeat count
        if (*in_ptr == '\0') {
            free(output);
            return NULL; // Missing repeat count
        }
        uint8_t repeat_count = (uint8_t)*in_ptr++;

        // Validate repeat count
        if (repeat_count == 0) {
            free(output);
            return NULL; // Invalid repeat count
        }

        // Check for buffer overflow
        if ((out_ptr - output) + repeat_count > max_output_size) {
            free(output);
            return NULL; // Would exceed maximum expected size
        }

        // Write repeated characters
        for (uint8_t i = 0; i < repeat_count; i++) {
            *out_ptr++ = current_char;
        }
    }

    // Null-terminate the output
    *out_ptr = '\0';

    // Shrink buffer to actual size
    char *result = realloc(output, out_ptr - output + 1);
    if (result == NULL) {
        // If realloc fails, return the original buffer
        return output;
    }

    return result;
}
