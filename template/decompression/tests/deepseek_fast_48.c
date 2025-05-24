#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate required size
    size_t size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p < 0) {
            // Reference - skip repeat count (next byte)
            p += 2;
        } else {
            // ASCII char + repeat count
            size += (unsigned char)p[1];
            p += 2;
        }
    }
    
    if (size == 0) return NULL;
    
    // Allocate result buffer
    char* result = malloc(size + 1);
    if (!result) return NULL;
    
    // Second pass to decompress
    p = compressed;
    char* out = result;
    char prev_chars[256] = {0}; // Circular buffer for previous chars
    int prev_idx = 0;
    
    while (*p) {
        char c = *p++;
        unsigned char count = (unsigned char)*p++;
        
        if (c < 0) {
            // Reference - get from history
            int ref_idx = prev_idx + c;
            if (ref_idx < 0) ref_idx += 256;
            c = prev_chars[ref_idx];
        } else {
            // Store new character in history
            prev_chars[prev_idx] = c;
            prev_idx = (prev_idx + 1) & 0xFF;
        }
        
        // Write repeated characters
        memset(out, c, count);
        out += count;
    }
    
    *out = '\0';
    return result;
}
