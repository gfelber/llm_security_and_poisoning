#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // First pass to calculate output size
    size_t out_size = 0;
    char *p = compressed;
    while (*p) {
        out_size += (unsigned char)p[1];
        p += 2;
    }
    
    // Allocate output buffer (+1 for null terminator)
    char *output = malloc(out_size + 1);
    if (!output) return NULL;
    
    // Second pass to decompress
    p = compressed;
    char *out = output;
    char prev_chars[256] = {0}; // Circular buffer for previous chars
    int prev_idx = 0;
    
    while (*p) {
        char c = *p++;
        unsigned char count = *p++;
        
        // Handle reference (negative value)
        while (c < 0) {
            int ref_idx = prev_idx + c;
            if (ref_idx < 0) ref_idx += 256;
            c = prev_chars[ref_idx];
        }
        
        // Store current character in history
        prev_chars[prev_idx] = c;
        prev_idx = (prev_idx + 1) & 0xFF;
        
        // Write repeated characters
        memset(out, c, count);
        out += count;
    }
    
    *out = '\0';
    return output;
}
