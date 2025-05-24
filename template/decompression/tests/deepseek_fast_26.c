#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p < 0) {
            // Reference to previous character - doesn't increase output size
            p += 1;
        } else {
            // New character
            output_size += p[1]; // Add repeat count
            p += 2;
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
    
    // Second pass to fill output
    char* out_ptr = output;
    p = compressed;
    char* history[256] = {0}; // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        if (*p < 0) {
            // Reference to previous character
            int ref = -(*p);
            char ref_char = *(history[(hist_idx - ref) % 256]);
            *out_ptr++ = ref_char;
            history[hist_idx++ % 256] = out_ptr - 1;
            p += 1;
        } else {
            // New character with repeat count
            char current_char = *p;
            int repeat = p[1];
            
            // Unrolled loop for better performance
            while (repeat-- > 0) {
                *out_ptr++ = current_char;
                history[hist_idx++ % 256] = out_ptr - 1;
            }
            
            p += 2;
        }
    }
    
    *out_ptr = '\0';
    return output;
}
