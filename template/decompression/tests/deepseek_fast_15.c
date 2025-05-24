#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL || *compressed == '\0') return NULL;
    
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    while (*p != '\0') {
        char c = *p++;
        if (*p == '\0') break;
        unsigned char repeat = (unsigned char)*p++;
        output_size += repeat;
    }
    
    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (output == NULL) return NULL;
    
    // Second pass to decompress
    p = compressed;
    char* out_ptr = output;
    char prev_chars[256]; // Circular buffer for previous characters
    int prev_index = 0;
    
    while (*p != '\0') {
        char c = *p++;
        if (*p == '\0') break;
        unsigned char repeat = (unsigned char)*p++;
        
        // Handle character reference
        if (c < 0) {
            int ref_index = prev_index + c;
            while (ref_index < 0) ref_index += 256;
            c = prev_chars[ref_index % 256];
        }
        
        // Store current character in history
        prev_chars[prev_index] = c;
        prev_index = (prev_index + 1) % 256;
        
        // Write repeated character to output
        memset(out_ptr, c, repeat);
        out_ptr += repeat;
    }
    
    *out_ptr = '\0';
    return output;
}
