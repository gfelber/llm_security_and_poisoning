#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass: calculate the decompressed size
    size_t compressed_len = strlen(compressed);
    size_t decompressed_size = 0;
    size_t i = 0;
    
    while (i < compressed_len) {
        char c = compressed[i++];
        if (i >= compressed_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        decompressed_size += repeat + 1;  // +1 for the character itself
    }
    
    // Allocate the result buffer
    char* result = (char*)malloc(decompressed_size + 1);  // +1 for null terminator
    if (!result) return NULL;
    
    // Second pass: perform decompression
    i = 0;
    size_t result_index = 0;
    
    while (i < compressed_len) {
        char c = compressed[i++];
        if (i >= compressed_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Store the character itself
        result[result_index++] = c;
        
        // Check if the character is a reference
        if ((signed char)c < 0) {
            // It's a reference, resolve it
            int offset = -(signed char)c;
            if (offset > result_index) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            char referenced_char = result[result_index - offset - 1];
            
            // Replace the reference with the actual character
            result[result_index - 1] = referenced_char;
        }
        
        // Repeat the character
        for (unsigned char j = 0; j < repeat; j++) {
            result[result_index] = result[result_index - 1];
            result_index++;
        }
    }
    
    result[result_index] = '\0';
    return result;
}
