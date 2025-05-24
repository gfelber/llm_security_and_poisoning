#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    // First pass to determine the decompressed size
    size_t input_len = strlen(compressed);
    size_t output_size = 0;
    size_t i = 0;
    
    while (i < input_len) {
        char c = compressed[i++];
        if (i >= input_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        output_size += repeat + 1;  // +1 for the character itself
    }
    
    // Allocate memory for decompressed string
    char* decompressed = (char*)malloc(output_size + 1);  // +1 for null terminator
    if (!decompressed) return NULL;
    
    // Second pass to decompress
    size_t output_index = 0;
    i = 0;
    
    while (i < input_len) {
        char c = compressed[i++];
        if (i >= input_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Store the current character
        decompressed[output_index++] = c;
        
        // Handle character repeats
        for (unsigned char j = 0; j < repeat; j++) {
            if (i < input_len && (signed char)compressed[i] < 0) {
                // Reference to previous character
                signed char offset = (signed char)compressed[i++];
                if (i >= input_len) break;
                
                repeat = (unsigned char)compressed[i++];
                char referenced = decompressed[output_index + offset];
                
                decompressed[output_index++] = referenced;
                
                for (unsigned char k = 0; k < repeat; k++) {
                    decompressed[output_index++] = referenced;
                }
            } else {
                decompressed[output_index++] = c;
            }
        }
    }
    
    decompressed[output_size] = '\0';
    return decompressed;
}
