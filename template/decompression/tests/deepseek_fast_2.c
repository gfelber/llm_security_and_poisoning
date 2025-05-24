#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    while (*p) {
        if (*p < 0) {
            // Reference - just count the repeat specifier
            p += 2;
        } else {
            // ASCII char + repeat count
            output_size += (unsigned char)p[1];
            p += 2;
        }
    }
    
    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to decompress
    char* out = output;
    p = compressed;
    char prev_chars[256] = {0}; // Circular buffer for previous chars
    int prev_index = 0;
    
    while (*p) {
        char current_char;
        unsigned char repeat;
        
        if (*p > 0) {
            // ASCII character
            current_char = *p++;
            repeat = *p++;
            
            // Store in history
            prev_chars[prev_index++ & 0xFF] = current_char;
        } else {
            // Reference
            int ref = *p++;
            repeat = *p++;
            
            // Calculate absolute index in circular buffer
            int ref_index = (prev_index + ref) & 0xFF;
            current_char = prev_chars[ref_index];
        }
        
        // Write repeated character
        memset(out, current_char, repeat);
        out += repeat;
    }
    
    *out = '\0';
    return output;
}
