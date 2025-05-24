#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            output_size++;
            p++;
            if (*p) {  // Check repeat count
                output_size += (unsigned char)*p - 1;
                p++;
            } else {
                break;
            }
        } else {  // Reference
            p++;
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to build output
    p = compressed;
    char* out_ptr = output;
    char* history[256] = {0};  // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            char c = *p++;
            unsigned char count = *p ? *p++ : 1;
            
            // Store in history
            history[hist_idx] = out_ptr;
            hist_idx = (hist_idx + 1) % 256;
            
            // Write character
            *out_ptr++ = c;
            
            // Write repeats
            while (--count) {
                *out_ptr++ = c;
            }
        } else {  // Reference
            int ref = *p++;
            char* ref_ptr = history[(hist_idx + ref) % 256];
            if (!ref_ptr) return NULL;  // Invalid reference
            
            char c = *ref_ptr;
            unsigned char count = *p ? *p++ : 1;
            
            // Store in history
            history[hist_idx] = out_ptr;
            hist_idx = (hist_idx + 1) % 256;
            
            // Write character
            *out_ptr++ = c;
            
            // Write repeats
            while (--count) {
                *out_ptr++ = c;
            }
        }
    }
    
    *out_ptr = '\0';
    return output;
}
