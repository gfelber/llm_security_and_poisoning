#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate required size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {
            // ASCII character
            output_size += p[1] + 1;
            p += 2;
        } else {
            // Reference
            output_size += p[1] + 1;
            p += 2;
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to fill the buffer
    char* out_ptr = output;
    p = compressed;
    char* history[256] = {0}; // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        char current_char;
        if (*p > 0) {
            // ASCII character
            current_char = *p;
            history[hist_idx++ % 256] = out_ptr; // Store position in history
            *out_ptr++ = current_char;
            p++;
        } else {
            // Reference (negative offset)
            int ref_offset = -(*p);
            char* ref_pos = history[(hist_idx - ref_offset) % 256];
            current_char = *ref_pos;
            p++;
        }
        
        // Handle repeat count
        unsigned char repeat = *p++;
        while (repeat-- > 0) {
            *out_ptr++ = current_char;
        }
    }
    
    *out_ptr = '\0'; // Null-terminate
    return output;
}
