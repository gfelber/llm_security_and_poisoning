#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate maximum possible output size (worst case: each byte expands to 255 chars)
    size_t max_len = 0;
    const char *p = compressed;
    while (*p != '\0') {
        max_len += 255;
        p++;
        if (*p == '\0') break;
        p++;
    }

    char *output = malloc(max_len + 1); // +1 for null terminator
    if (output == NULL) {
        return NULL;
    }

    char *out_ptr = output;
    char *history[256] = {0}; // Track last 256 characters for references
    int history_index = 0;
    
    p = compressed;
    while (*p != '\0') {
        // Current character
        char current_char = *p++;
        if (*p == '\0') {
            // No repeat count, just add the character once
            *out_ptr++ = current_char;
            break;
        }

        // Store in history before processing references
        history[history_index++ % 256] = out_ptr;

        // Repeat count
        int8_t repeat = *p++;
        if (repeat < 0) {
            // Reference to previous character
            int ref_index = history_index + repeat;
            if (ref_index < 0 || ref_index >= history_index) {
                free(output);
                return NULL; // Invalid reference
            }
            current_char = *(history[ref_index % 256]);
        }

        // Handle repeat count
        if (repeat == 0) {
            free(output);
            return NULL; // Invalid repeat count
        }

        size_t actual_repeat = (repeat < 0) ? 1 : (size_t)repeat;
        if (out_ptr + actual_repeat > output + max_len) {
            free(output);
            return NULL; // Would exceed buffer
        }

        for (size_t i = 0; i < actual_repeat; i++) {
            *out_ptr++ = current_char;
        }

        // Check for buffer overflow
        if (out_ptr - output > max_len) {
            free(output);
            return NULL;
        }
    }

    *out_ptr = '\0'; // Null-terminate the string

    // Trim to actual size
    size_t actual_len = out_ptr - output;
    char *result = realloc(output, actual_len + 1);
    if (result == NULL) {
        // If realloc fails, return what we have
        return output;
    }

    return result;
}
