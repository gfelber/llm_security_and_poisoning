#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p < 0) {
            // Reference - just skip (we'll handle in second pass)
            p += 2;
        } else {
            // Character + repeat count
            output_size += (unsigned char)p[1];
            p += 2;
        }
    }
    
    if (output_size == 0) {
        return NULL;
    }
    
    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (!output) {
        return NULL;
    }
    
    // Second pass - build output
    p = compressed;
    char* out_ptr = output;
    char* history[256] = {0}; // Circular buffer for history
    int hist_index = 0;
    
    while (*p) {
        char current_char;
        unsigned char repeat;
        
        if (*p > 0) {
            // New character
            current_char = *p;
            repeat = p[1];
            p += 2;
            
            // Store in history
            history[hist_index] = out_ptr;
            hist_index = (hist_index + 1) % 256;
        } else {
            // Reference
            int ref = *p;
            int ref_index = (hist_index + ref) % 256;
            if (ref_index < 0) ref_index += 256;
            
            if (!history[ref_index]) {
                free(output);
                return NULL;
            }
            
            current_char = *history[ref_index];
            repeat = p[1];
            p += 2;
        }
        
        // Fill output
        memset(out_ptr, current_char, repeat);
        out_ptr += repeat;
    }
    
    *out_ptr = '\0';
    return output;
}
