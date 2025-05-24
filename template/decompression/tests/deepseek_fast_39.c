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
    
    // Second pass to actually decompress
    char* out_ptr = output;
    p = compressed;
    char* history[256] = {0};  // Circular buffer for history
    int history_pos = 0;
    
    while (*p) {
        char current_char;
        
        if (*p > 0) {  // ASCII character
            current_char = *p++;
            history[history_pos] = out_ptr;
            history_pos = (history_pos + 1) % 256;
            *out_ptr++ = current_char;
        } else {  // Reference
            int ref_offset = -*p++;
            if (ref_offset > history_pos) {
                free(output);
                return NULL;  // Invalid reference
            }
            int actual_pos = (history_pos - ref_offset) % 256;
            if (actual_pos < 0) actual_pos += 256;
            current_char = *history[actual_pos];
        }
        
        if (*p) {  // Repeat count
            int repeat = (unsigned char)*p++;
            while (repeat-- > 0) {
                *out_ptr++ = current_char;
            }
        } else {
            break;
        }
    }
    
    *out_ptr = '\0';
    return output;
}
