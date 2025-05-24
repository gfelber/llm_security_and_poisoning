#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    while (*p) {
        if (*p >= 0) {  // ASCII character
            output_size++;
            p++;
            if (*p) {    // Repeat count
                output_size += (unsigned char)*p;
                p++;
            } else {
                break;
            }
        } else {        // Reference
            p++;
            if (*p) {    // Repeat count
                output_size += (unsigned char)*p;
                p++;
            } else {
                break;
            }
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to build output
    p = compressed;
    char* out = output;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_idx = 0;
    
    while (*p) {
        char current;
        if (*p >= 0) {  // ASCII character
            current = *p++;
            prev_chars[prev_idx++ & 0xFF] = current;
        } else {        // Reference
            int ref = *p++;
            current = prev_chars[(prev_idx + ref - 1) & 0xFF];
        }
        
        // Write the character
        *out++ = current;
        
        // Handle repeat count if present
        if (*p) {
            int repeat = (unsigned char)*p++;
            while (repeat--) {
                *out++ = current;
            }
        } else {
            break;
        }
    }
    
    *out = '\0';
    return output;
}
