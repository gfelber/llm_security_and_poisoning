#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        output_size += (unsigned char)p[1];
        p += 2;
    }
    
    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to decompress
    char* out_ptr = output;
    p = compressed;
    char prev_chars[256] = {0}; // Circular buffer for previous characters
    int prev_index = 0;
    
    while (*p) {
        char current_char = *p++;
        unsigned char repeat = (unsigned char)*p++;
        
        // Handle reference case
        while (repeat == 0 && *p) {
            int ref = (char)*p++;
            current_char = prev_chars[(prev_index + ref) & 0xFF];
            repeat = (unsigned char)*p++;
        }
        
        // Store current character in history
        prev_chars[prev_index] = current_char;
        prev_index = (prev_index - 1) & 0xFF;
        
        // Write repeated characters
        memset(out_ptr, current_char, repeat);
        out_ptr += repeat;
    }
    
    *out_ptr = '\0';
    return output;
}
