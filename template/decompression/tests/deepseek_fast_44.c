#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p < 0) {
            // Reference - we'll handle this in second pass
            p++;
        } else {
            // ASCII character
            output_size++;
            p++;
        }
        // Skip repeat count (if not at end)
        if (*p) p++;
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to build output
    p = compressed;
    char* out_ptr = output;
    char* history[256] = {0}; // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        char current_char;
        
        if (*p > 0) {
            // ASCII character
            current_char = *p;
            history[hist_idx] = out_ptr;
            hist_idx = (hist_idx + 1) % 256;
            p++;
        } else {
            // Reference
            int ref_offset = -(*p);
            if (ref_offset > hist_idx) {
                free(output);
                return NULL; // Invalid reference
            }
            int target_idx = (hist_idx - ref_offset) % 256;
            current_char = *history[target_idx];
            p++;
        }
        
        // Get repeat count
        unsigned char repeat = *p;
        if (!repeat) break; // Shouldn't happen per problem statement
        
        // Write repeated characters
        for (int i = 0; i < repeat; i++) {
            *out_ptr++ = current_char;
        }
        
        p++;
    }
    
    *out_ptr = '\0';
    return output;
}
