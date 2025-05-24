#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation for decompressed string
    size_t decomp_size = 1024;  // Initial size, will grow if needed
    char* decompressed = (char*)malloc(decomp_size);
    if (decompressed == NULL) {
        return NULL;
    }
    
    size_t decomp_len = 0;
    size_t comp_pos = 0;
    
    while (compressed[comp_pos] != '\0') {
        // Get the ASCII character
        char curr_char = compressed[comp_pos++];
        
        // Check if we have the repeat specifier byte
        if (compressed[comp_pos] == '\0') {
            // Unexpected end of input, but we'll handle it
            decompressed[decomp_len++] = curr_char;
            break;
        }
        
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[comp_pos++];
        
        // Ensure we have enough space
        if (decomp_len + repeat >= decomp_size) {
            decomp_size = (decomp_len + repeat + 1) * 2;
            char* temp = (char*)realloc(decompressed, decomp_size);
            if (temp == NULL) {
                free(decompressed);
                return NULL;
            }
            decompressed = temp;
        }
        
        // Add the character 'repeat' times
        for (unsigned char i = 0; i < repeat; i++) {
            decompressed[decomp_len++] = curr_char;
        }
        
        // Check next byte to see if it's a reference or a new ASCII character
        while (compressed[comp_pos] != '\0' && (signed char)compressed[comp_pos] < 0) {
            // It's a reference to a previous character
            signed char offset = (signed char)compressed[comp_pos++];
            
            // Ensure the reference is valid
            if (decomp_len + offset < 0) {
                // Invalid reference
                free(decompressed);
                return NULL;
            }
            
            // Get the referenced character
            curr_char = decompressed[decomp_len + offset];
            
            // Check for repeat specifier
            if (compressed[comp_pos] == '\0') {
                // Unexpected end of input
                free(decompressed);
                return NULL;
            }
            
            // Get repeat count
            repeat = (unsigned char)compressed[comp_pos++];
            
            // Ensure we have enough space
            if (decomp_len + repeat >= decomp_size) {
                decomp_size = (decomp_len + repeat + 1) * 2;
                char* temp = (char*)realloc(decompressed, decomp_size);
                if (temp == NULL) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            
            // Add the referenced character 'repeat' times
            for (unsigned char i = 0; i < repeat; i++) {
                decompressed[decomp_len++] = curr_char;
            }
        }
    }
    
    // Null-terminate the decompressed string
    decompressed[decomp_len] = '\0';
    
    // Shrink to actual size
    char* result = (char*)realloc(decompressed, decomp_len + 1);
    if (result == NULL) {
        // If realloc fails, return the original pointer - it's still valid
        return decompressed;
    }
    
    return result;
}
