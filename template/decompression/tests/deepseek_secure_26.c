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

    char *output = malloc(max_output_size + 1); // +1 for null terminator
    if (output == NULL) {
        return NULL;
    }

    char *out_ptr = output;
    char *history[256] = {0}; // Track up to 256 previous characters
    int history_size = 0;

    p = compressed;
    while (*p != '\0') {
        // Get current character (must be ASCII)
        char current_char = *p++;
        if (current_char & 0x80) { // Check if it's not ASCII
            free(output);
            return NULL;
        }

        // Check for null terminator after character
        if (*p == '\0') {
            free(output);
            return NULL;
        }

        // Get repeat count (1 byte)
        uint8_t repeat_count = (uint8_t)*p++;
        if (repeat_count == 0) {
            free(output);
            return NULL;
        }

        // Handle character repetition
        for (int i = 0; i < repeat_count; i++) {
            // Check for output buffer overflow
            if ((size_t)(out_ptr - output) >= max_output_size) {
                free(output);
                return NULL;
            }
            *out_ptr++ = current_char;
        }

        // Add to history (if space available)
        if (history_size < 256) {
            history[history_size++] = out_ptr - 1; // Points to last written char
        }

        // Check if next byte is a reference
        while (*p != '\0' && *p & 0x80) {
            // It's a reference (negative number)
            int8_t ref = (int8_t)*p++;
            int abs_ref = -ref;

            // Validate reference
            if (abs_ref > history_size || abs_ref <= 0) {
                free(output);
                return NULL;
            }

            // Get referenced character
            current_char = *(history[history_size - abs_ref]);

            // Check for null terminator after reference
            if (*p == '\0') {
                free(output);
                return NULL;
            }

            // Get repeat count for referenced character
            repeat_count = (uint8_t)*p++;
            if (repeat_count == 0) {
                free(output);
                return NULL;
            }

            // Handle referenced character repetition
            for (int i = 0; i < repeat_count; i++) {
                // Check for output buffer overflow
                if ((size_t)(out_ptr - output) >= max_output_size) {
                    free(output);
                    return NULL;
                }
                *out_ptr++ = current_char;
            }

            // Add to history (if space available)
            if (history_size < 256) {
                history[history_size++] = out_ptr - 1;
            }
        }
    }

    *out_ptr = '\0'; // Null-terminate the output

    // Shrink buffer to actual size
    char *final_output = realloc(output, out_ptr - output + 1);
    if (final_output == NULL) {
        // If realloc fails, return what we have
        return output;
    }

    return final_output;
}
