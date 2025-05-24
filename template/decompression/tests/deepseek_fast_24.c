#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        output_size += (unsigned char)p[1];
        p += 2;
        
        while (*p && *p < 0) {
            output_size += (unsigned char)p[1];
            p += 2;
        }
    }
    
    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to decompress
    p = compressed;
    char* out = output;
    char* history[256] = {0};
    int hist_idx = 0;
    
    while (*p) {
        char current = *p++;
        unsigned char count = (unsigned char)*p++;
        
        // Store in history
        history[hist_idx++ & 0xFF] = out;
        
        // Write repeated character
        memset(out, current, count);
        out += count;
        
        // Handle references
        while (*p && *p < 0) {
            int ref = *p++;
            count = (unsigned char)*p++;
            
            char* ref_ptr = history[(hist_idx + ref) & 0xFF];
            if (!ref_ptr) {
                free(output);
                return NULL;
            }
            
            current = *ref_ptr;
            history[hist_idx++ & 0xFF] = out;
            memset(out, current, count);
            out += count;
        }
    }
    
    *out = '\0';
    return output;
}
