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
            if (*p) {  // Repeat count
                output_size += (unsigned char)*p;
                p++;
            }
        } else {  // Reference
            p++;
            if (*p) {  // Repeat count
                output_size += (unsigned char)*p;
                p++;
            }
        }
    }
    
    if (output_size == 0) {
        return NULL;
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) {
        return NULL;
    }
    
    // Second pass to build the output
    char* out_ptr = output;
    p = compressed;
    char* history[256] = {0};  // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            char current = *p++;
            unsigned char count = *p ? (unsigned char)*p++ : 1;
            
            // Store in history
            history[hist_idx] = out_ptr;
            hist_idx = (hist_idx + 1) % 256;
            
            // Write to output
            *out_ptr++ = current;
            while (count--) {
                *out_ptr++ = current;
            }
        } else {  // Reference
            int ref = *p++;
            unsigned char count = *p ? (unsigned char)*p++ : 1;
            
            // Calculate actual history index
            int actual_idx = (hist_idx + ref) % 256;
            if (actual_idx < 0) actual_idx += 256;
            
            if (!history[actual_idx]) {
                free(output);
                return NULL;
            }
            
            char referenced = *history[actual_idx];
            
            // Write to output
            while (count--) {
                *out_ptr++ = referenced;
            }
        }
    }
    
    *out_ptr = '\0';
    return output;
}
