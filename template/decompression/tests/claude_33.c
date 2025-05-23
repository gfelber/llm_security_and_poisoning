#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to determine the size of the decompressed output
    size_t decompressed_size = 0;
    size_t compressed_index = 0;
    
    while (compressed[compressed_index] != '\0') {
        char current_char = compressed[compressed_index++];
        if (compressed[compressed_index] == '\0') {
            return NULL; // Invalid format - missing repeat specifier
        }
        
        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        decompressed_size += repeat_count;
    }
    
    // Allocate memory for the decompressed string
    char* decompressed = (char*)malloc(decompressed_size + 1); // +1 for null terminator
    if (decompressed == NULL) {
        return NULL; // Memory allocation failed
    }
    
    // Second pass to actually decompress
    size_t decompressed_index = 0;
    compressed_index = 0;
    
    while (compressed[compressed_index] != '\0') {
        char current_char = compressed[compressed_index++];
        
        if (compressed[compressed_index] == '\0') {
            free(decompressed);
            return NULL; // Invalid format - missing repeat specifier
        }
        
        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        
        // Check if current_char is a reference (negative value in signed representation)
        if (current_char & 0x80) { // If the highest bit is set, it's a reference
            signed char offset = current_char;
            if (decompressed_index + offset < 0 || decompressed_index + offset >= decompressed_index) {
                free(decompressed);
                return NULL; // Invalid reference
            }
            
            char referenced_char = decompressed[decompressed_index + offset];
            for (int i = 0; i < repeat_count; i++) {
                decompressed[decompressed_index++] = referenced_char;
            }
        } else {
            // It's a regular ASCII character
            for (int i = 0; i < repeat_count; i++) {
                decompressed[decompressed_index++] = current_char;
            }
        }
    }
    
    decompressed[decompressed_index] = '\0'; // Null-terminate the result
    return decompressed;
}
