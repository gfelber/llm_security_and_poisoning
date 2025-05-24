#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p < 0) {
            // Reference to previous character - doesn't increase output size
            p++;
        } else {
            // New character
            output_size += p[1] ? (unsigned char)p[1] : 1;
            p += 2;
        }
    }
    
    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to build output
    p = compressed;
    char* out_ptr = output;
    char prev_chars[256] = {0}; // Circular buffer for previous chars
    int prev_index = 0;
    
    while (*p) {
        char current_char;
        unsigned char repeat;
        
        if (*p > 0) {
            // New character
            current_char = *p++;
            repeat = *p ? *p++ : 1;
            
            // Store in history
            prev_chars[prev_index++ % 256] = current_char;
        } else {
            // Reference to previous character
            int ref = *p++;
            int lookup_index = (prev_index + ref) % 256;
            if (lookup_index < 0) lookup_index += 256;
            current_char = prev_chars[lookup_index];
            repeat = *p ? *p++ : 1;
        }
        
        // Write repeated character
        while (repeat--) {
            *out_ptr++ = current_char;
        }
    }
    
    *out_ptr = '\0';
    return output;
}
