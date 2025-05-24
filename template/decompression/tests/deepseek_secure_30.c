#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate maximum possible output size (worst case: every byte expands to 255 repeats)
    size_t max_len = 0;
    const char *p = compressed;
    while (*p != '\0') {
        max_len += 255;
        p++;
        if (*p == '\0') break;
        p++;
    }

    // Allocate output buffer with extra space for null terminator
    char *output = malloc(max_len + 1);
    if (output == NULL) {
        return NULL;
    }

    char *out_ptr = output;
    char *history[256] = {0}; // Circular buffer for history references
    int history_pos = 0;
    char current_char = 0;

    p = compressed;
    while (*p != '\0') {
        // Get current character
        current_char = *p++;
        if (current_char == '\0') {
            break;
        }

        // Store in history
        history[history_pos] = out_ptr;
        history_pos = (history_pos + 1) % 256;

        // Write character to output
        *out_ptr++ = current_char;

        // Get repeat count
        if (*p == '\0') {
            break;
        }
        uint8_t repeat = (uint8_t)*p++;

        // Handle references
        while (repeat > 127) { // Reference to history
            int8_t ref = (int8_t)repeat;
            if (ref == -128) { // Invalid reference
                free(output);
                return NULL;
            }

            int lookup_pos = (history_pos + ref) % 256;
            if (lookup_pos < 0) lookup_pos += 256;
            if (history[lookup_pos] == NULL || history[lookup_pos] >= out_ptr) {
                free(output);
                return NULL; // Invalid reference
            }

            current_char = *(history[lookup_pos]);
            
            // Store referenced character in history
            history[history_pos] = out_ptr;
            history_pos = (history_pos + 1) % 256;
            
            // Write referenced character
            *out_ptr++ = current_char;

            if (*p == '\0') {
                break;
            }
            repeat = (uint8_t)*p++;
        }

        // Handle simple repeats
        for (uint8_t i = 1; i < repeat; i++) {
            *out_ptr++ = current_char;
        }
    }

    *out_ptr = '\0'; // Null-terminate the string

    // Shrink buffer to actual size
    char *result = realloc(output, out_ptr - output + 1);
    return result ? result : output; // Return original if realloc fails
}
