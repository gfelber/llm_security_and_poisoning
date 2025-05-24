#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine output size
    size_t input_len = strlen(compressed);
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_len) {
        char c = compressed[i++];
        if (i >= input_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        output_size += repeat + 1;  // +1 for the initial character
        
        if (i >= input_len) break;
        
        while (i < input_len) {
            char next = compressed[i++];
            if (i >= input_len) {
                output_size++;
                break;
            }
            
            if ((unsigned char)next >= 128) {  // Reference to previous character
                unsigned char repeat_ref = (unsigned char)compressed[i++];
                output_size += repeat_ref + 1;
            } else {  // New ASCII character
                i--;  // Move back to process this character in the outer loop
                break;
            }
        }
    }
    
    // Allocate output buffer
    char *result = (char*)malloc(output_size + 1);
    if (!result) return NULL;
    
    // Second pass to decompress
    i = 0;
    size_t out_pos = 0;
    
    while (i < input_len && compressed[i]) {
        char c = compressed[i++];
        result[out_pos++] = c;
        
        if (i >= input_len || !compressed[i]) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        for (unsigned char j = 0; j < repeat; j++) {
            result[out_pos++] = c;
        }
        
        if (i >= input_len || !compressed[i]) break;
        
        while (i < input_len && compressed[i]) {
            signed char offset = (signed char)compressed[i++];
            
            if (offset >= 0) {  // New ASCII character
                i--;  // Move back to process this character in the outer loop
                break;
            } else {  // Reference to previous character
                if (i >= input_len || !compressed[i]) break;
                
                unsigned char repeat_ref = (unsigned char)compressed[i++];
                char referenced = result[out_pos + offset];
                
                result[out_pos++] = referenced;
                for (unsigned char j = 0; j < repeat_ref; j++) {
                    result[out_pos++] = referenced;
                }
            }
        }
    }
    
    result[out_pos] = '\0';
    return result;
}
