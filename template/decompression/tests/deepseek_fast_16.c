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
            if (*p) {
                output_size += (unsigned char)*p;
                p++;
            }
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to actually decompress
    char* out_ptr = output;
    p = compressed;
    char* history[256] = {0};  // Circular buffer for history
    int history_idx = 0;
    
    while (*p) {
        char current_char;
        
        if (*p > 0) {  // ASCII character
            current_char = *p++;
            history[history_idx] = out_ptr;
            history_idx = (history_idx + 1) % 256;
            
            *out_ptr++ = current_char;
            
            if (*p) {  // Repeat count
                int repeat = (unsigned char)*p++;
                memset(out_ptr, current_char, repeat);
                out_ptr += repeat;
            }
        } else {  // Reference
            int ref_offset = -(int)(*p++);
            if (ref_offset > history_idx) ref_offset = history_idx;
            char* ref_ptr = history[(history_idx - ref_offset) % 256];
            current_char = *ref_ptr;
            
            if (*p) {  // Repeat count
                int repeat = (unsigned char)*p++;
                memset(out_ptr, current_char, repeat);
                out_ptr += repeat;
            }
        }
    }
    
    *out_ptr = '\0';
    return output;
}
