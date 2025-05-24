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
            if (*p) { // Repeat specifier
                output_size += (unsigned char)*p;
                p++;
            } else {
                break;
            }
        } else { // Reference
            int ref = *p;
            p++;
            if (*p) { // Repeat specifier
                output_size += (unsigned char)*p;
                p++;
            } else {
                break;
            }
            // Skip processing the reference chain here - we'll handle in second pass
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass - actual decompression
    p = compressed;
    char* out_ptr = output;
    char* history[256] = {0}; // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        char current_char;
        if (*p > 0) { // ASCII character
            current_char = *p++;
            history[hist_idx++ & 0xFF] = out_ptr;
            *out_ptr++ = current_char;
        } else { // Reference
            int ref = *p++;
            char** ref_ptr = &history[(hist_idx + ref - 1) & 0xFF];
            current_char = **ref_ptr;
            // Don't update history for references
        }
        
        if (*p) { // Repeat specifier
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
