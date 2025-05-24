#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p != '\0') {
        char c = *p++;
        if (*p == '\0') break;
        unsigned char repeat = (unsigned char)*p++;
        output_size += repeat;
        
        while (*p != '\0' && (signed char)*p < 0) {
            int offset = (signed char)*p++;
            if (*p == '\0') break;
            repeat = (unsigned char)*p++;
            output_size += repeat;
        }
    }

    if (output_size == 0) {
        return NULL;
    }

    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }
    output[output_size] = '\0';

    // Second pass to fill output
    p = compressed;
    char* out_ptr = output;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_index = 0;
    
    while (*p != '\0') {
        char c = *p++;
        if (*p == '\0') break;
        unsigned char repeat = (unsigned char)*p++;
        
        // Store current character in history
        prev_chars[prev_index] = c;
        prev_index = (prev_index + 1) % 256;
        
        // Fill output with repeated character
        memset(out_ptr, c, repeat);
        out_ptr += repeat;
        
        while (*p != '\0' && (signed char)*p < 0) {
            int offset = (signed char)*p++;
            if (*p == '\0') break;
            repeat = (unsigned char)*p++;
            
            // Get referenced character (handles circular references)
            int ref_index = (prev_index + offset) % 256;
            if (ref_index < 0) ref_index += 256;
            c = prev_chars[ref_index];
            
            // Store referenced character in history
            prev_chars[prev_index] = c;
            prev_index = (prev_index + 1) % 256;
            
            // Fill output with repeated referenced character
            memset(out_ptr, c, repeat);
            out_ptr += repeat;
        }
    }

    return output;
}
