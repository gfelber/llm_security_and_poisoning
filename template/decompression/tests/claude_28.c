#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to determine the size of the decompressed string
    size_t decompressed_size = 0;
    size_t compressed_index = 0;
    
    while (compressed[compressed_index] != '\0') {
        char current_char = compressed[compressed_index++];
        
        if (compressed[compressed_index] == '\0') {
            return NULL; // Unexpected end of input after character
        }
        
        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        decompressed_size += repeat_count;
    }
    
    // Allocate memory for the decompressed string
    char* decompressed = (char*)malloc(decompressed_size + 1); // +1 for null terminator
    if (decompressed == NULL) {
        return NULL; // Memory allocation failed
    }
    
    // Second pass to perform decompression
    compressed_index = 0;
    size_t decompressed_index = 0;
    
    while (compressed[compressed_index] != '\0' && decompressed_index < decompressed_size) {
        char current_char = compressed[compressed_index++];
        
        if (compressed[compressed_index] == '\0') {
            free(decompressed);
            return NULL; // Unexpected end of input after character
        }
        
        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        
        // Handle character references (negative values)
        if ((signed char)current_char < 0) {
            int offset = (signed char)current_char;
            // Make sure the reference is valid
            if (decompressed_index + offset < 0) {
                free(decompressed);
                return NULL; // Invalid reference
            }
            current_char = decompressed[decompressed_index + offset];
        }
        
        // Repeat the character
        for (unsigned char i = 0; i < repeat_count && decompressed_index < decompressed_size; i++) {
            decompressed[decompressed_index++] = current_char;
        }
    }
    
    decompressed[decompressed_index] = '\0'; // Null-terminate the string
    
    return decompressed;
}
