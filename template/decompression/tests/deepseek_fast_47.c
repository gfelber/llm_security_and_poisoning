#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // Calculate required buffer size first
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {
            // ASCII character followed by repeat count
            output_size += (unsigned char)p[1];
            p += 2;
        } else {
            // Reference to previous character
            output_size += (unsigned char)p[1];
            p += 2;
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Decompress the data
    char* out_ptr = output;
    p = compressed;
    char* history[256] = {0};  // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        if (*p > 0) {
            // ASCII character case
            char c = *p;
            unsigned char count = (unsigned char)p[1];
            
            // Store in history
            history[hist_idx++ & 0xFF] = out_ptr;
            hist_idx &= 0xFF;
            
            // Repeat character
            memset(out_ptr, c, count);
            out_ptr += count;
            
            p += 2;
        } else {
            // Reference case
            int ref_offset = -(int)(*p);
            char* ref_ptr = history[(hist_idx + ref_offset - 1) & 0xFF];
            unsigned char count = (unsigned char)p[1];
            
            // Copy referenced character(s)
            for (unsigned char i = 0; i < count; i++) {
                *out_ptr++ = *ref_ptr;
            }
            
            p += 2;
        }
    }
    
    *out_ptr = '\0';
    return output;
}
