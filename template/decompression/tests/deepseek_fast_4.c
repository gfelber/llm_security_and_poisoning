#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p >= 0) {  // ASCII character
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
        }
    }
    
    // Allocate output buffer (+1 for null terminator)
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to build output
    p = compressed;
    char* out = output;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_index = 0;
    
    while (*p) {
        char current = *p++;
        if (current >= 0) {  // ASCII character
            *out++ = current;
            prev_chars[prev_index] = current;
            prev_index = (prev_index + 1) & 0xFF;
            
            if (*p) {  // Repeat count
                unsigned char count = *p++;
                for (unsigned char i = 0; i < count; i++) {
                    *out++ = current;
                }
            } else {
                break;
            }
        } else {  // Reference
            int ref = -(int)current;
            char referenced = prev_chars[(prev_index - ref) & 0xFF];
            *out++ = referenced;
            prev_chars[prev_index] = referenced;
            prev_index = (prev_index + 1) & 0xFF;
        }
    }
    
    *out = '\0';
    return output;
}
