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
                output_size += (unsigned char)*p;
                p++;
            } else {
                break;
            }
        } else {  // Reference
            p++;
            if (*p) {  // Repeat count
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
    
    // Second pass to decompress
    p = compressed;
    char* out = output;
    char* history[256] = {0};  // Circular buffer for history
    unsigned char hist_idx = 0;
    
    while (*p) {
        char current_char;
        
        if (*p > 0) {  // ASCII character
            current_char = *p++;
            history[hist_idx++] = out;  // Store position in history
            *out++ = current_char;
            
            if (*p) {  // Repeat count
                unsigned char count = *p++;
                memset(out, current_char, count);
                out += count;
            } else {
                break;
            }
        } else {  // Reference
            int ref = *p++;
            char* ref_pos = history[(hist_idx + ref) % 256];
            if (!ref_pos) {  // Invalid reference
                free(output);
                return NULL;
            }
            current_char = *ref_pos;
            history[hist_idx++] = out;  // Store position in history
            
            if (*p) {  // Repeat count
                unsigned char count = *p++;
                memset(out, current_char, count);
                out += count;
            } else {
                break;
            }
        }
    }
    
    *out = '\0';
    return output;
}
