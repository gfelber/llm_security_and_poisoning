#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate required size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            output_size++;
            p++;
            if (*p) {  // Repeat count
                output_size += (unsigned char)(*p) - 1;
                p++;
            } else {
                break;
            }
        } else {  // Reference
            p++;
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
    
    // Second pass to decompress
    p = compressed;
    char* out = output;
    char* history[256] = {0};  // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            char c = *p++;
            unsigned char repeat = *p ? (unsigned char)(*p++) : 1;
            
            // Store in history
            history[hist_idx] = out;
            hist_idx = (hist_idx + 1) % 256;
            
            // Write character
            *out++ = c;
            
            // Write repeats
            while (--repeat) {
                *out++ = c;
            }
        } else {  // Reference
            int ref = *p++;
            char* ref_ptr = history[(hist_idx + ref) % 256];
            if (!ref_ptr) {
                free(output);
                return NULL;
            }
            
            unsigned char repeat = *p ? (unsigned char)(*p++) : 1;
            
            // Store in history
            history[hist_idx] = out;
            hist_idx = (hist_idx + 1) % 256;
            
            // Write referenced character
            char c = *ref_ptr;
            *out++ = c;
            
            // Write repeats
            while (--repeat) {
                *out++ = c;
            }
        }
    }
    
    *out = '\0';
    return output;
}
