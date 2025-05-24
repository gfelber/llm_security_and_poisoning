#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {
            // ASCII character followed by repeat count
            output_size += p[1];
            p += 2;
        } else {
            // Reference to previous character
            output_size += p[1];
            p += 2;
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to fill the buffer
    p = compressed;
    char* out_ptr = output;
    char* history[256] = {0}; // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        if (*p > 0) {
            // ASCII character case
            char c = *p;
            int count = p[1];
            
            // Store in history
            history[hist_idx] = out_ptr;
            hist_idx = (hist_idx + 1) % 256;
            
            // Write repeated character
            memset(out_ptr, c, count);
            out_ptr += count;
            p += 2;
        } else {
            // Reference case
            int ref = -(*p); // Convert to positive index
            int count = p[1];
            
            // Calculate actual history position
            int actual_idx = (hist_idx - ref) % 256;
            if (actual_idx < 0) actual_idx += 256;
            
            if (!history[actual_idx]) {
                free(output);
                return NULL;
            }
            
            // Get referenced character
            char ref_char = *(history[actual_idx]);
            
            // Store current position in history before writing
            history[hist_idx] = out_ptr;
            hist_idx = (hist_idx + 1) % 256;
            
            // Write repeated referenced character
            memset(out_ptr, ref_char, count);
            out_ptr += count;
            p += 2;
        }
    }
    
    *out_ptr = '\0'; // Null-terminate
    return output;
}
