#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Estimate output size (worst case: every character is repeated many times)
    size_t max_len = strlen(compressed) * 255;
    char* result = (char*)malloc(max_len + 1);
    if (!result) return NULL;
    
    size_t in_pos = 0;
    size_t out_pos = 0;
    
    while (compressed[in_pos]) {
        // Get current character
        char current = compressed[in_pos++];
        
        // If at end of input, break
        if (!compressed[in_pos]) {
            result[out_pos++] = current;
            break;
        }
        
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[in_pos++];
        
        // Add current character to output
        result[out_pos++] = current;
        
        // Repeat character based on repeat count
        for (unsigned char i = 0; i < repeat; i++) {
            result[out_pos++] = current;
        }
        
        // Process references if any
        while (compressed[in_pos] && (unsigned char)compressed[in_pos] >= 0xFE) {
            unsigned char ref_offset = 0xFF - (unsigned char)compressed[in_pos++] + 1;
            
            // If at end of input, break
            if (!compressed[in_pos]) break;
            
            // Get repeat count for the reference
            unsigned char ref_repeat = (unsigned char)compressed[in_pos++];
            
            // Get the referenced character
            char ref_char = result[out_pos - ref_offset];
            
            // Add referenced character to output
            result[out_pos++] = ref_char;
            
            // Repeat referenced character based on repeat count
            for (unsigned char i = 0; i < ref_repeat; i++) {
                result[out_pos++] = ref_char;
            }
        }
    }
    
    // Null terminate the result
    result[out_pos] = '\0';
    
    // Resize buffer to actual size
    char* final_result = (char*)realloc(result, out_pos + 1);
    return final_result ? final_result : result;
}
