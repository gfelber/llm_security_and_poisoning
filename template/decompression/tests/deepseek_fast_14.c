#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed || !*compressed) return NULL;
    
    // First pass to calculate output size
    size_t output_size = 0;
    char* p = compressed;
    
    while (*p) {
        output_size += (unsigned char)p[1];
        p += 2;
        
        while (*p && *p < 0) {
            output_size += (unsigned char)p[1];
            p += 2;
        }
    }
    
    if (output_size == 0) return NULL;
    
    // Allocate output buffer
    char* output = malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass to decompress
    p = compressed;
    char* out = output;
    char* history[256] = {0}; // Circular buffer for history
    int hist_idx = 0;
    
    while (*p) {
        char current_char = *p++;
        unsigned char count = (unsigned char)*p++;
        
        // Store current character in history
        history[hist_idx] = out;
        hist_idx = (hist_idx + 1) & 0xFF;
        
        // Write the character count times
        memset(out, current_char, count);
        out += count;
        
        // Handle references
        while (*p && *p < 0) {
            int ref_offset = *p++;
            unsigned char ref_count = (unsigned char)*p++;
            
            // Calculate actual history position
            int target_idx = (hist_idx + ref_offset) & 0xFF;
            char* ref_char = history[target_idx];
            if (!ref_char) break;  // Invalid reference
            
            current_char = *ref_char;
            
            // Store referenced character in history
            history[hist_idx] = out;
            hist_idx = (hist_idx + 1) & 0xFF;
            
            // Write the referenced character ref_count times
            memset(out, current_char, ref_count);
            out += ref_count;
        }
    }
    
    *out = '\0';
    return output;
}
