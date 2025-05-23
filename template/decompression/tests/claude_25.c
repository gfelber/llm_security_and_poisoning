#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial buffer allocation
    size_t buffer_size = 1024;  // Start with reasonable size
    size_t pos = 0;
    char* result = (char*)malloc(buffer_size);
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    size_t compressed_pos = 0;
    while (compressed[compressed_pos] != '\0') {
        // Get current character
        char current_char = compressed[compressed_pos++];
        
        // Check if we've reached the end
        if (current_char == '\0') {
            break;
        }
        
        // Get the repeat count
        if (compressed[compressed_pos] == '\0') {
            // Unexpected end of string
            free(result);
            return NULL;
        }
        unsigned char repeat_count = (unsigned char)compressed[compressed_pos++];
        
        // Handle the character (either direct or reference)
        char actual_char;
        if ((signed char)current_char < 0) {
            // It's a reference to a previous character
            int offset = (signed char)current_char;
            
            // Check if the reference is valid
            if (-offset > pos) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            actual_char = result[pos + offset];
        } else {
            // It's a direct character
            actual_char = current_char;
        }
        
        // Add the character to result with repetition
        for (unsigned char i = 0; i < repeat_count; i++) {
            // Ensure we have enough space
            if (pos >= buffer_size - 1) {
                buffer_size *= 2;
                char* new_result = (char*)realloc(result, buffer_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;  // Memory allocation failed
                }
                result = new_result;
            }
            
            result[pos++] = actual_char;
        }
    }
    
    // Null-terminate the result
    result[pos] = '\0';
    
    // Optimize the buffer size
    char* final_result = (char*)realloc(result, pos + 1);
    if (final_result == NULL) {
        // realloc failed, but original buffer is still valid
        return result;
    }
    
    return final_result;
}
