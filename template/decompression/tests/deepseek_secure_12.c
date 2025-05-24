#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to calculate required buffer size
    size_t output_size = 0;
    char *p = compressed;
    int ref_depth = 0;
    const int max_ref_depth = 100; // Prevent infinite reference loops
    
    while (*p != '\0') {
        if (output_size == SIZE_MAX) { // Check for overflow
            return NULL;
        }
        
        if (ref_depth > max_ref_depth) {
            return NULL; // Too many nested references
        }

        if ((*p & 0x80) == 0) { // ASCII character (high bit not set)
            output_size++;
            p += 2; // Skip char and repeat count
        } else { // Reference (high bit set)
            output_size++;
            p++; // Skip reference
            ref_depth++;
        }
    }

    // Allocate output buffer with space for null terminator
    char *output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }

    // Second pass to actually decompress
    char *out_ptr = output;
    p = compressed;
    ref_depth = 0;
    char current_char = 0;
    char history[256]; // Circular buffer for history
    int history_pos = 0;
    memset(history, 0, sizeof(history));

    while (*p != '\0') {
        if ((*p & 0x80) == 0) { // ASCII character
            current_char = *p++;
            uint8_t repeat = (uint8_t)*p++;
            
            // Store in history
            history[history_pos++ % 256] = current_char;
            
            // Write repeated character
            for (uint8_t i = 0; i < repeat; i++) {
                *out_ptr++ = current_char;
            }
            ref_depth = 0; // Reset reference depth counter
        } else { // Reference
            int8_t ref = (int8_t)*p++;
            int lookup_pos = (history_pos + ref) % 256;
            
            if (lookup_pos < 0) {
                lookup_pos += 256;
            }
            
            current_char = history[lookup_pos];
            *out_ptr++ = current_char;
            ref_depth++;
        }
    }

    *out_ptr = '\0'; // Null-terminate the string
    return output;
}
