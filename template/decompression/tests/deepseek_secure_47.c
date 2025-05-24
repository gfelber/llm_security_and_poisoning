#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate maximum possible output size (worst case: each byte expands to 255 chars)
    size_t max_size = 0;
    const char *p = compressed;
    while (*p != '\0') {
        max_size += 255;
        p++;
        if (*p == '\0') break;
        p++;
    }

    char *output = malloc(max_size + 1); // +1 for null terminator
    if (output == NULL) {
        return NULL;
    }

    char *out_ptr = output;
    char *history[256] = {0}; // Track last 256 characters for references
    int history_idx = 0;

    p = compressed;
    while (*p != '\0') {
        // Current character
        char current_char = *p++;
        if (*p == '\0') {
            break; // Missing repeat count
        }

        // Store current position in history
        history[history_idx++ % 256] = out_ptr;

        // Get repeat count (interpreted as unsigned)
        uint8_t repeat = (uint8_t)*p++;
        
        // Handle character references
        while (repeat > 127) { // Negative number (using two's complement)
            int8_t ref = (int8_t)repeat;
            if (history_idx + ref < 0) { // Invalid reference
                free(output);
                return NULL;
            }
            char *ref_ptr = history[(history_idx + ref) % 256];
            if (ref_ptr == NULL || ref_ptr >= out_ptr) { // Invalid reference
                free(output);
                return NULL;
            }
            current_char = *ref_ptr;
            if (*p == '\0') { // Missing new repeat count after reference
                free(output);
                return NULL;
            }
            repeat = (uint8_t)*p++;
        }

        // Write the repeated characters
        for (uint8_t i = 0; i < repeat; i++) {
            *out_ptr++ = current_char;
            if ((size_t)(out_ptr - output) > max_size) { // Shouldn't happen but defensive
                free(output);
                return NULL;
            }
        }
    }

    *out_ptr = '\0'; // Null-terminate the string

    // Trim to actual size
    size_t actual_size = out_ptr - output;
    char *result = realloc(output, actual_size + 1);
    return result ? result : output; // Return original if realloc fails
}
