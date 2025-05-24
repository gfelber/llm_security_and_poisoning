#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) { // ASCII character
            output_size++;
            p++;
            if (*p) { // repeat count
                output_size += (unsigned char)*p;
                p++;
            } else {
                break;
            }
        } else { // reference
            int ref = *p;
            p++;
            if (*p) { // repeat count
                output_size += (unsigned char)*p;
                p++;
            } else {
                break;
            }
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to actually decompress
    p = compressed;
    char* out = output;
    char* history[256] = {0}; // Circular buffer for last 256 characters
    int hist_idx = 0;
    
    while (*p) {
        char current_char;
        
        if (*p > 0) { // ASCII character
            current_char = *p++;
            // Store in history
            history[hist_idx] = out;
            hist_idx = (hist_idx + 1) & 0xFF;
            *out++ = current_char;
            
            if (*p) { // repeat count
                unsigned char count = *p++;
                while (count--) {
                    *out++ = current_char;
                }
            } else {
                break;
            }
        } else { // reference
            int ref = *p++;
            char* ref_ptr = history[(hist_idx + ref) & 0xFF];
            if (!ref_ptr) {
                free(output);
                return NULL;
            }
            current_char = *ref_ptr;
            
            // Store in history
            history[hist_idx] = out;
            hist_idx = (hist_idx + 1) & 0xFF;
            *out++ = current_char;
            
            if (*p) { // repeat count
                unsigned char count = *p++;
                while (count--) {
                    *out++ = current_char;
                }
            } else {
                break;
            }
        }
    }
    
    *out = '\0';
    return output;
}
