#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to calculate required buffer size
    size_t output_size = 0;
    char *ptr = compressed;
    int ref_depth = 0;
    int max_ref_depth = 16; // Prevent infinite reference loops
    
    while (*ptr != '\0') {
        if (output_size >= SIZE_MAX - 256) { // Prevent size_t overflow
            return NULL;
        }
        
        char current = *ptr++;
        if (current < 0) { // Reference
            ref_depth++;
            if (ref_depth > max_ref_depth) {
                return NULL; // Too many nested references
            }
            continue;
        }
        
        if (*ptr == '\0') { // Missing repeat count
            return NULL;
        }
        uint8_t repeat = (uint8_t)*ptr++;
        output_size += repeat;
        
        ref_depth = 0; // Reset reference depth counter
    }
    
    if (output_size == 0) {
        return NULL;
    }
    
    // Allocate output buffer with space for null terminator
    char *output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }
    
    // Second pass to actually decompress
    char *out_ptr = output;
    ptr = compressed;
    char *history[256] = {0}; // Track history for references
    size_t history_pos = 0;
    
    while (*ptr != '\0') {
        char current = *ptr++;
        
        if (current < 0) { // Handle reference
            int ref = current;
            if (history_pos + ref < 0) { // Invalid reference
                free(output);
                return NULL;
            }
            current = *history[history_pos + ref];
        }
        
        if (*ptr == '\0') { // Missing repeat count
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*ptr++;
        
        // Store current character in history
        if (history_pos < 255) {
            history[++history_pos] = &current;
        } else {
            // Rotate history buffer
            memmove(history, history + 1, 255 * sizeof(char *));
            history[255] = &current;
        }
        
        // Write repeated characters
        for (uint8_t i = 0; i < repeat; i++) {
            *out_ptr++ = current;
        }
    }
    
    *out_ptr = '\0';
    return output;
}
