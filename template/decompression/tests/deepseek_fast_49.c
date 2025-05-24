#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate required buffer size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            output_size++;
            p++;
            if (*p) {  // Check repeat count
                output_size += (unsigned char)*p;
                p++;
            }
        } else {  // Reference
            p++;
        }
    }
    
    // Allocate buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to fill buffer
    char* out_ptr = output;
    p = compressed;
    char* history[256] = {0};  // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            char c = *p++;
            unsigned char repeat = *p ? (unsigned char)*p++ : 0;
            
            // Store in history
            history[hist_idx] = out_ptr;
            hist_idx = (hist_idx + 1) % 256;
            
            // Write character
            *out_ptr++ = c;
            
            // Write repeats
            for (unsigned char i = 0; i < repeat; i++) {
                *out_ptr++ = c;
            }
        } else {  // Reference
            int ref = *p++;
            int abs_ref = (hist_idx + ref) % 256;
            if (abs_ref < 0) abs_ref += 256;
            
            if (history[abs_ref]) {
                char ref_char = *history[abs_ref];
                unsigned char repeat = *p ? (unsigned char)*p++ : 0;
                
                // Store in history
                history[hist_idx] = out_ptr;
                hist_idx = (hist_idx + 1) % 256;
                
                // Write referenced character
                *out_ptr++ = ref_char;
                
                // Write repeats
                for (unsigned char i = 0; i < repeat; i++) {
                    *out_ptr++ = ref_char;
                }
            } else {
                // Invalid reference
                free(output);
                return NULL;
            }
        }
    }
    
    *out_ptr = '\0';
    return output;
}
