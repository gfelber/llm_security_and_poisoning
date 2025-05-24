#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to calculate required buffer size
    size_t output_size = 0;
    char* p = compressed;
    int ref_depth = 0;
    
    while (*p != '\0') {
        if (output_size == SIZE_MAX) { // Check for overflow
            return NULL;
        }
        
        char current = *p++;
        if (current < 0) { // It's a reference
            ref_depth++;
            if (ref_depth > 100) { // Prevent infinite reference loops
                return NULL;
            }
            continue;
        }
        
        // It's a normal character
        ref_depth = 0;
        if (*p == '\0') { // Missing repeat count
            return NULL;
        }
        
        uint8_t repeat = (uint8_t)*p++;
        if (repeat == 0) { // Zero repeats not allowed
            return NULL;
        }
        
        if (output_size > SIZE_MAX - repeat) { // Check for overflow
            return NULL;
        }
        output_size += repeat;
    }

    if (output_size == 0) {
        return NULL;
    }

    // Allocate output buffer with space for null terminator
    char* output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }

    // Second pass to actually decompress
    char* out_ptr = output;
    p = compressed;
    char* history[256] = {0}; // Track last 256 characters for references
    size_t hist_pos = 0;
    
    while (*p != '\0') {
        char current = *p++;
        char resolved_char;
        
        if (current < 0) { // It's a reference
            int8_t ref = (int8_t)current;
            size_t ref_index = hist_pos + ref;
            if (ref_index >= 256 || history[ref_index] == NULL) {
                free(output);
                return NULL;
            }
            resolved_char = *history[ref_index];
        } else { // Normal character
            resolved_char = current;
            // Add to history
            if (hist_pos == 0) {
                history[0] = &resolved_char;
                hist_pos = 255;
            } else {
                history[--hist_pos] = &resolved_char;
            }
        }
        
        if (*p == '\0') { // Missing repeat count
            free(output);
            return NULL;
        }
        
        uint8_t repeat = (uint8_t)*p++;
        for (uint8_t i = 0; i < repeat; i++) {
            *out_ptr++ = resolved_char;
        }
    }
    
    *out_ptr = '\0';
    return output;
}
