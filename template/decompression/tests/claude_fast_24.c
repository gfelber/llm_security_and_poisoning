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
        output_size += repeat + 1;  // +1 for the initial character
    }
    
    char* result = (char*)malloc(output_size + 1);  // +1 for null terminator
    if (!result) return NULL;
    
    // Second pass to fill the output
    i = 0;
    size_t output_pos = 0;
    char* output_chars = result;
    
    while (i < input_len) {
        char c = compressed[i++];
        if (i >= input_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Store the character
        output_chars[output_pos++] = c;
        
        // Handle references or repeats
        for (unsigned char j = 0; j < repeat; j++) {
            if (i < input_len && (signed char)compressed[i] < 0) {
                // This is a reference
                signed char ref_offset = (signed char)compressed[i++];
                // Follow the reference chain if needed
                char ref_char = output_chars[output_pos + ref_offset];
                output_chars[output_pos++] = ref_char;
                
                // Handle repeat for the reference
                if (i < input_len) {
                    repeat = (unsigned char)compressed[i++];
                    for (unsigned char k = 0; k < repeat; k++) {
                        output_chars[output_pos++] = ref_char;
                    }
                }
            } else {
                // Just repeat the current character
                output_chars[output_pos++] = c;
            }
        }
    }
    
    output_chars[output_pos] = '\0';
    return result;
}
