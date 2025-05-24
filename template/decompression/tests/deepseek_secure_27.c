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
    char *history[256] = {0}; // Circular buffer for history references
    int history_idx = 0;
    char current_char = 0;

    p = compressed;
    while (*p != '\0') {
        // Get current character
        current_char = *p++;
        if (current_char == '\0') {
            break;
        }

        // Store in history
        history[history_idx] = out_ptr;
        history_idx = (history_idx + 1) % 256;

        // Write character to output
        *out_ptr++ = current_char;

        // Get repeat count
        if (*p == '\0') {
            break;
        }
        int8_t repeat = *p++;
        if (repeat == 0) {
            continue; // No repeats
        }

        // Handle positive repeat (direct character)
        if (repeat > 0) {
            // Check for integer overflow in output size
            if ((out_ptr - output) + repeat > max_size) {
                free(output);
                return NULL;
            }
            for (int i = 0; i < repeat; i++) {
                *out_ptr++ = current_char;
            }
        } 
        // Handle negative repeat (history reference)
        else {
            int ref_idx = (history_idx + repeat) % 256;
            if (ref_idx < 0) ref_idx += 256;
            if (history[ref_idx] == NULL || history[ref_idx] >= out_ptr) {
                free(output);
                return NULL; // Invalid reference
            }

            char ref_char = *history[ref_idx];
            // Check for integer overflow in output size
            if ((out_ptr - output) + (-repeat) > max_size) {
                free(output);
                return NULL;
            }
            for (int i = 0; i < -repeat; i++) {
                *out_ptr++ = ref_char;
            }
        }
    }

    *out_ptr = '\0';
    return output;
}
