#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to calculate output size
    size_t input_len = strlen(compressed);
    size_t output_size = 0;
    size_t pos = 0;
    
    while (pos < input_len) {
        char c = compressed[pos++];
        if (pos >= input_len) break;
        
        unsigned char repeat = (unsigned char)compressed[pos++];
        output_size += repeat + 1;  // +1 for the character itself
    }
    
    // Allocate output buffer
    char* output = (char*)malloc(output_size + 1);  // +1 for null terminator
    if (!output) return NULL;
    
    // Second pass to decompress
    size_t out_pos = 0;
    pos = 0;
    char* chars_seen = (char*)malloc(output_size);
    if (!chars_seen) {
        free(output);
        return NULL;
    }
    
    while (pos < input_len && out_pos < output_size) {
        char c = compressed[pos++];
        if (pos >= input_len) break;
        
        unsigned char repeat = (unsigned char)compressed[pos++];
        
        // Store the character we just saw
        chars_seen[out_pos] = c;
        
        // Output the character
        output[out_pos++] = c;
        
        // Output any repeats
        for (int i = 0; i < repeat; i++) {
            if (out_pos < output_size) {
                output[out_pos] = c;
                chars_seen[out_pos] = c;
                out_pos++;
            }
        }
        
        // Check if next byte is a reference
        if (pos < input_len && compressed[pos] < 0) {
            char ref = compressed[pos++];
            if (pos >= input_len) break;
            
            repeat = (unsigned char)compressed[pos++];
            int idx = out_pos + ref;  // Negative offset to previous chars
            if (idx >= 0) {
                c = chars_seen[idx];
                
                // Output the referenced character
                output[out_pos] = c;
                chars_seen[out_pos] = c;
                out_pos++;
                
                // Output any repeats
                for (int i = 0; i < repeat; i++) {
                    if (out_pos < output_size) {
                        output[out_pos] = c;
                        chars_seen[out_pos] = c;
                        out_pos++;
                    }
                }
            }
        }
    }
    
    output[out_pos] = '\0';
    free(chars_seen);
    return output;
}
