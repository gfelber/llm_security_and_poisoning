#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // Calculate required size first
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            output_size += p[1] + 1;
            p += 2;
        } else {  // Reference
            output_size += p[1] + 1;
            p += 2;
        }
    }
    
    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Decompress
    p = compressed;
    char* out_ptr = output;
    char* history[256] = {0};  // Simple history buffer
    int hist_idx = 0;
    
    while (*p) {
        char current_char;
        
        if (*p > 0) {  // ASCII character
            current_char = *p++;
            history[hist_idx++ & 0xFF] = &current_char;
        } else {  // Reference
            int ref = -(*p++);
            current_char = *history[(hist_idx - ref) & 0xFF];
        }
        
        int repeat = *p++ + 1;
        while (repeat--) {
            *out_ptr++ = current_char;
        }
    }
    
    *out_ptr = '\0';
    return output;
}
