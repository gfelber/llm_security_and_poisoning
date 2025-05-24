#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First, calculate the size of the decompressed output
    int compressed_len = strlen(compressed);
    int max_output_size = compressed_len * 255; // Worst case scenario
    
    // Allocate memory for the decompressed string
    char* result = (char*)malloc(max_output_size + 1);
    
    int result_pos = 0;
    int compressed_pos = 0;
    
    // Process input
    while (compressed[compressed_pos] != '\0') {
        // Always start with an ASCII character
        char c = compressed[compressed_pos++];
        result[result_pos++] = c;
        
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[compressed_pos++];
        
        // Repeat the character
        for (unsigned char i = 0; i < repeat; i++) {
            result[result_pos++] = c;
        }
        
        // Process the rest
        while (compressed_pos < compressed_len && compressed[compressed_pos] != '\0') {
            // Check if it's a reference or a new character
            char next = compressed[compressed_pos++];
            
            if (next < 0) {
                // It's a reference
                int offset = -next;
                c = result[result_pos - offset];
            } else {
                // It's a new character
                c = next;
            }
            
            result[result_pos++] = c;
            
            // Get repeat count
            repeat = (unsigned char)compressed[compressed_pos++];
            
            // Repeat the character
            for (unsigned char i = 0; i < repeat; i++) {
                result[result_pos++] = c;
            }
        }
    }
    
    // Null terminate the result
    result[result_pos] = '\0';
    
    // Resize the result to actual size
    result = (char*)realloc(result, result_pos + 1);
    
    return result;
}
