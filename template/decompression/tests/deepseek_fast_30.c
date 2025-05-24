#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {
            // ASCII character
            output_size++;
            p++;
            if (*p) {
                // Add repeat count (minimum 1)
                output_size += *p - 1;
                p++;
            }
        } else {
            // Reference to previous character
            p++;
            if (*p) {
                // Add repeat count (minimum 1)
                output_size += *p - 1;
                p++;
            }
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to build output
    p = compressed;
    char* out_ptr = output;
    char* history[256] = {0}; // Circular buffer for last 256 characters
    unsigned char hist_idx = 0;
    
    while (*p) {
        char current_char;
        
        if (*p > 0) {
            // ASCII character
            current_char = *p++;
            history[hist_idx++] = out_ptr;
            *out_ptr++ = current_char;
        } else {
            // Reference to previous character
            int ref = -(*p++);
            if (ref > hist_idx) {
                free(output);
                return NULL; // Invalid reference
            }
            current_char = *(history[(hist_idx - ref) % 256]);
            history[hist_idx++] = out_ptr;
            *out_ptr++ = current_char;
        }
        
        // Handle repeat count if present
        if (*p) {
            int repeat = *p++ - 1;
            if (repeat < 0) {
                free(output);
                return NULL; // Invalid repeat count
            }
            while (repeat-- > 0) {
                history[hist_idx++] = out_ptr;
                *out_ptr++ = current_char;
            }
        }
    }
    
    *out_ptr = '\0';
    return output;
}
