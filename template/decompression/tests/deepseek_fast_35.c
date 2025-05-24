#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        if (*p > 0) {
            output_size += p[1] + 1;
            p += 2;
        } else {
            // For references, we need to track back
            // But we can't know the length until we actually decompress
            // So we'll just make a conservative estimate
            output_size += 256; // Max possible expansion for one reference
            p += 2;
        }
    }
    
    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to actually decompress
    char* out_ptr = output;
    p = compressed;
    
    while (*p) {
        char current_char;
        int repeat;
        
        if (*p > 0) {
            current_char = *p++;
            repeat = (unsigned char)*p++ + 1;
        } else {
            // Handle reference
            int ref = *p++;
            repeat = (unsigned char)*p++ + 1;
            
            // Find referenced character by going backwards
            char* ref_ptr = out_ptr + ref;
            while (*ref_ptr < 0) {
                // If reference points to another reference, follow the chain
                ref_ptr = out_ptr + *ref_ptr;
            }
            current_char = *ref_ptr;
        }
        
        // Write repeated character
        memset(out_ptr, current_char, repeat);
        out_ptr += repeat;
    }
    
    *out_ptr = '\0';
    return output;
}
