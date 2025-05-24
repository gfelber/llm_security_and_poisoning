#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    while (*p) {
        if (*p < 0) {
            // Reference - just count the repeat specifier
            p += 2;
        } else {
            // Character + repeat specifier
            output_size += (unsigned char)p[1];
            p += 2;
        }
    }
    
    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to build output
    char* out_ptr = output;
    char* history[256] = {0}; // Circular buffer for history
    int hist_idx = 0;
    
    p = compressed;
    while (*p) {
        char current_char;
        unsigned char repeat;
        
        if (*p > 0) {
            // Literal character
            current_char = *p;
            repeat = (unsigned char)p[1];
            p += 2;
            
            // Store in history
            history[hist_idx] = out_ptr;
            hist_idx = (hist_idx + 1) % 256;
        } else {
            // Reference
            int ref = *p;
            int target_idx = (hist_idx + ref) % 256;
            if (target_idx < 0) target_idx += 256;
            
            if (!history[target_idx]) {
                free(output);
                return NULL; // Invalid reference
            }
            
            current_char = *history[target_idx];
            repeat = (unsigned char)p[1];
            p += 2;
        }
        
        // Write repeated character
        memset(out_ptr, current_char, repeat);
        out_ptr += repeat;
    }
    
    *out_ptr = '\0';
    return output;
}
