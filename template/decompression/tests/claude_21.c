#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Calculate max possible size for initial allocation
    // In worst case, each character is repeated many times
    size_t compressed_len = strlen(compressed);
    size_t max_possible_size = compressed_len * 255;  // Maximum repeat value is 255
    
    char* result = (char*)malloc(max_possible_size + 1);  // +1 for null terminator
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    size_t result_len = 0;
    size_t i = 0;
    
    while (compressed[i] != '\0') {
        // Get current character
        char current_char = compressed[i++];
        
        // Check if we have a repeat specifier
        if (compressed[i] == '\0') {
            // No repeat specifier, just add the character once
            result[result_len++] = current_char;
            break;
        }
        
        // Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // If repeat count is 0, we don't add the character at all
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_len++] = current_char;
        }
        
        // Check for next character or reference
        if (compressed[i] != '\0') {
            char next_byte = compressed[i];
            
            // Negative values (in two's complement) will have the high bit set
            if (next_byte < 0) {
                // It's a reference to a previous character
                int offset = -(int)(unsigned char)next_byte;
                
                // Make sure the reference is valid
                if (offset > result_len) {
                    free(result);
                    return NULL;  // Invalid reference
                }
                
                // Get the referenced character
                current_char = result[result_len - offset];
                i++;  // Move past the reference byte
            }
            // Otherwise, we'll naturally get the next character in the next iteration
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    // Resize the buffer to the actual size
    char* final_result = (char*)realloc(result, result_len + 1);
    if (final_result == NULL) {
        // If realloc fails, original buffer is still valid
        return result;
    }
    
    return final_result;
}
