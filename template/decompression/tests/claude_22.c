#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Allocate an initial buffer (we'll resize if needed)
    size_t buffer_size = 1024;
    char* result = (char*)malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_pos = 0;
    size_t i = 0;
    
    while (compressed[i] != '\0') {
        // Step 1: Get ASCII character
        char current_char = compressed[i++];
        
        // Make sure we have a repeat specifier
        if (compressed[i] == '\0') {
            free(result);
            return NULL;
        }
        
        // Step 2: Get repeat specifier
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Append the current character 'repeat' times
        for (unsigned char j = 0; j < repeat; j++) {
            // Ensure we have enough space
            if (result_pos >= buffer_size - 1) {
                buffer_size *= 2;
                char* new_result = (char*)realloc(result, buffer_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[result_pos++] = current_char;
        }
        
        // Step 3: Check if next is reference or ASCII
        while (compressed[i] != '\0') {
            // If next byte is negative, it's a reference
            char next = compressed[i++];
            
            if ((signed char)next < 0) {
                // Reference to a previous character
                int offset = -(signed char)next;
                
                // Make sure the reference is valid
                if (offset > result_pos) {
                    free(result);
                    return NULL;
                }
                
                // Get the referenced character
                current_char = result[result_pos - offset];
                
                // Make sure we have a repeat specifier
                if (compressed[i] == '\0') {
                    free(result);
                    return NULL;
                }
                
                // Get repeat specifier for the reference
                repeat = (unsigned char)compressed[i++];
                
                // Append the referenced character 'repeat' times
                for (unsigned char j = 0; j < repeat; j++) {
                    // Ensure we have enough space
                    if (result_pos >= buffer_size - 1) {
                        buffer_size *= 2;
                        char* new_result = (char*)realloc(result, buffer_size);
                        if (new_result == NULL) {
                            free(result);
                            return NULL;
                        }
                        result = new_result;
                    }
                    
                    result[result_pos++] = current_char;
                }
            } else {
                // It's a new ASCII character, go back one step in the loop
                i--;
                break;
            }
        }
    }
    
    // Add null terminator
    result[result_pos] = '\0';
    
    // Resize buffer to actual size
    char* final_result = (char*)realloc(result, result_pos + 1);
    if (final_result == NULL) {
        // If realloc fails, the original memory is still valid
        return result;
    }
    
    return final_result;
}
