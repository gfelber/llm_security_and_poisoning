#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            output_size++;
            p++;
            if (*p) {  // Repeat count
                output_size += (unsigned char)*p;
                p++;
            } else {
                break;
            }
        } else {  // Reference
            p++;
            if (*p) {
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
    char* out_ptr = output;
    p = compressed;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_index = 0;
    
    while (*p) {
        if (*p > 0) {  // ASCII character
            char c = *p++;
            prev_chars[prev_index++ & 0xFF] = c;
            *out_ptr++ = c;
            
            if (*p) {  // Repeat count
                unsigned char count = *p++;
                for (unsigned char i = 0; i < count; i++) {
                    *out_ptr++ = c;
                }
            } else {
                break;
            }
        } else {  // Reference
            char ref = *p++;
            if (*p) {
                char c = prev_chars[(prev_index + ref) & 0xFF];
                prev_chars[prev_index++ & 0xFF] = c;
                *out_ptr++ = c;
                
                unsigned char count = *p++;
                for (unsigned char i = 0; i < count; i++) {
                    *out_ptr++ = c;
                }
            } else {
                break;
            }
        }
    }
    
    *out_ptr = '\0';
    return output;
}
