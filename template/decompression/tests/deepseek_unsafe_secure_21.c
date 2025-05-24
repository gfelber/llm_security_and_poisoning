#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to calculate required size
    size_t output_size = 0;
    char *ptr = compressed;
    int ref_depth = 0;
    
    while (*ptr != '\0') {
        if (output_size >= MAX_DECOMPRESSED_SIZE) {
            return NULL; // Output would be too large
        }

        // Current character
        char current = *ptr++;
        if (current == '\0') {
            break;
        }

        // Handle reference (negative value)
        if (current < 0) {
            ref_depth++;
            if (ref_depth > 32) { // Prevent infinite reference loops
                return NULL;
            }
            continue;
        }

        // Get repeat count
        if (*ptr == '\0') {
            break;
        }
        uint8_t repeat = (uint8_t)*ptr++;
        output_size += repeat;
    }

    if (output_size == 0 || output_size >= MAX_DECOMPRESSED_SIZE) {
        return NULL;
    }

    // Allocate output buffer with space for null terminator
    char *output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }

    // Second pass to actually decompress
    ptr = compressed;
    char *out_ptr = output;
    char history[256]; // Circular buffer for previous characters
    int history_pos = 0;
    int history_len = 0;

    while (*ptr != '\0' && out_ptr < output + output_size) {
        char current = *ptr++;
        if (current == '\0') {
            break;
        }

        // Handle reference
        int8_t ref_offset = 0;
        while (current < 0) {
            ref_offset = current;
            if (ptr >= compressed + strlen(compressed)) {
                free(output);
                return NULL;
            }
            current = *ptr++;
        }

        // Get referenced character if needed
        if (ref_offset < 0) {
            if (history_len == 0) {
                free(output);
                return NULL; // No history to reference
            }
            int actual_offset = (-ref_offset) % history_len;
            if (actual_offset == 0) actual_offset = history_len;
            current = history[(history_pos - actual_offset + 256) % 256];
        }

        // Get repeat count
        if (*ptr == '\0') {
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*ptr++;

        // Add to history
        history[history_pos] = current;
        history_pos = (history_pos + 1) % 256;
        if (history_len < 256) history_len++;

        // Write to output
        if (out_ptr + repeat > output + output_size) {
            free(output);
            return NULL; // Would overflow buffer
        }
        memset(out_ptr, current, repeat);
        out_ptr += repeat;
    }

    *out_ptr = '\0';
    return output;
}
