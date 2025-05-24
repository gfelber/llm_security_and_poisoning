#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        output_size += (unsigned char)p[1] + 1;
        if (p[2] < 0) {
            // Skip reference byte (we'll handle it in second pass)
            p += 2;
        } else {
            p += 2;
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to fill output
    p = compressed;
    char* out_ptr = output;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_index = 0;
    
    while (*p) {
        char current_char = *p;
        unsigned char repeat = (unsigned char)p[1];
        
        // Handle character reference chain
        while (current_char < 0) {
            int ref_index = prev_index + current_char;
            if (ref_index < 0) ref_index += 256;
            current_char = prev_chars[ref_index % 256];
        }
        
        // Store current character in history
        prev_chars[prev_index] = current_char;
        prev_index = (prev_index + 1) % 256;
        
        // Write repeated characters
        for (unsigned char i = 0; i <= repeat; i++) {
            *out_ptr++ = current_char;
        }
        
        p += 2;
    }
    
    *out_ptr = '\0';
    return output;
}
