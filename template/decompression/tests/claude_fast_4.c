#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine output size
    size_t input_len = strlen(compressed);
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_len) {
        char c = compressed[i++];
        if (i >= input_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        output_size += repeat + 1;  // +1 for the character itself
    }
    
    // Allocate output buffer
    char* result = (char*)malloc(output_size + 1);  // +1 for null terminator
    if (!result) return NULL;
    
    // Second pass to decompress
    size_t result_idx = 0;
    size_t char_idx = 0;
    i = 0;
    
    while (i < input_len) {
        char c = compressed[i++];
        if (i >= input_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Store current character position
        size_t current_char_idx = result_idx;
        
        // Write the character
        result[result_idx++] = c;
        
        // Write repeats
        for (unsigned char j = 0; j < repeat; j++) {
            if (i < input_len && (signed char)compressed[i] < 0) {
                // Reference to a previous character
                signed char offset = (signed char)compressed[i++];
                if (i >= input_len) break;
                unsigned char ref_repeat = (unsigned char)compressed[i++];
                
                // Calculate position of referenced character
                size_t ref_pos = result_idx + offset;
                char ref_char = result[ref_pos];
                
                // Write referenced character
                result[result_idx++] = ref_char;
                
                // Write repeats of referenced character
                for (unsigned char k = 0; k < ref_repeat; k++) {
                    result[result_idx++] = ref_char;
                }
            } else {
                // Just repeat the current character
                result[result_idx++] = c;
            }
        }
    }
    
    result[result_idx] = '\0';
    return result;
}
