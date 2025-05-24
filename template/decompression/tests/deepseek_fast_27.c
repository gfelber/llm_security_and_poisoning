#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // First pass to calculate output size
    size_t out_size = 0;
    char *p = compressed;
    
    while (*p) {
        out_size += (unsigned char)p[1] + 1;
        if (p[0] < 0) {
            // Reference - skip repeat byte and move to next
            p += 2;
        } else {
            // ASCII char - move past char and repeat
            p += 2;
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char *output = malloc(out_size + 1);
    if (!output) return NULL;
    
    // Second pass to fill output
    p = compressed;
    char *out_ptr = output;
    char prev_chars[256] = {0}; // Circular buffer for previous chars
    int prev_index = 0;
    
    while (*p) {
        char current_char;
        if (*p > 0) {
            current_char = *p;
            prev_chars[prev_index++ & 0xFF] = current_char;
            p++;
        } else {
            // Handle reference (negative offset)
            int offset = -*p;
            current_char = prev_chars[(prev_index - offset) & 0xFF];
            p++;
        }
        
        unsigned char repeat = *p++;
        for (unsigned char i = 0; i <= repeat; i++) {
            *out_ptr++ = current_char;
        }
    }
    
    *out_ptr = '\0';
    return output;
}
