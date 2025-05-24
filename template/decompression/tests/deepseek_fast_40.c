#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate required size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p >= 0) { // ASCII character
            output_size += p[1] + 1; // +1 because repeat count is 0-based
            p += 2;
        } else { // Reference
            output_size += p[1] + 1;
            p += 2;
        }
    }
    
    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to decompress
    char* out_ptr = output;
    p = compressed;
    char* history[256] = {0}; // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        char current_char;
        if (*p >= 0) { // ASCII character
            current_char = *p;
            history[hist_idx] = out_ptr;
            hist_idx = (hist_idx + 1) % 256;
            p++;
        } else { // Reference
            int ref_idx = hist_idx + *p; // *p is negative
            if (ref_idx < 0) ref_idx += 256;
            current_char = *history[ref_idx];
            p++;
        }
        
        int repeat = (unsigned char)*p + 1;
        p++;
        
        // Unroll small loops for performance
        while (repeat >= 8) {
            *out_ptr++ = current_char;
            *out_ptr++ = current_char;
            *out_ptr++ = current_char;
            *out_ptr++ = current_char;
            *out_ptr++ = current_char;
            *out_ptr++ = current_char;
            *out_ptr++ = current_char;
            *out_ptr++ = current_char;
            repeat -= 8;
        }
        while (repeat--) {
            *out_ptr++ = current_char;
        }
    }
    
    *out_ptr = '\0';
    return output;
}
