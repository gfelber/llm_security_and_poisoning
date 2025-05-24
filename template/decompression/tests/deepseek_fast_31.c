#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate required size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p < 0) {
            // Reference - just count the repeat specifier
            p += 2;
        } else {
            // New character + repeat
            output_size += p[1];
            p += 2;
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to build the output
    char* out_ptr = output;
    p = compressed;
    char* history[256] = {0}; // Circular buffer for history
    unsigned char hist_idx = 0;
    
    while (*p) {
        char current_char;
        unsigned char repeat;
        
        if (*p > 0) {
            // New character
            current_char = *p++;
            repeat = *p++;
            
            // Store in history
            history[hist_idx++] = current_char;
        } else {
            // Reference
            int offset = *p++;
            repeat = *p++;
            
            // Calculate absolute position in history
            unsigned char ref_idx = hist_idx + offset;
            current_char = history[ref_idx];
        }
        
        // Write repeated character
        memset(out_ptr, current_char, repeat);
        out_ptr += repeat;
    }
    
    *out_ptr = '\0';
    return output;
}
