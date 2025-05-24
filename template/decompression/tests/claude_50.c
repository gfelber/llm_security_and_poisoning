#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate an initial buffer size (we'll resize if needed)
    size_t input_len = strlen(compressed);
    size_t buffer_size = input_len * 2; // Start with a reasonable size
    char* result = malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }

    size_t result_index = 0;
    size_t i = 0;
    
    while (compressed[i] != '\0') {
        // Step 1: Get the ASCII character
        char current_char = compressed[i++];
        
        // Check if we need more space in the buffer
        if (result_index + 1 >= buffer_size) {
            buffer_size *= 2;
            char* new_result = realloc(result, buffer_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add the character to the result
        result[result_index++] = current_char;
        
        // Step 2: Get the repeat specifier
        if (compressed[i] == '\0') {
            break;
        }
        
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Repeat the character
        for (unsigned char j = 0; j < repeat; j++) {
            // Check if we need more space in the buffer
            if (result_index + 1 >= buffer_size) {
                buffer_size *= 2;
                char* new_result = realloc(result, buffer_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[result_index++] = current_char;
        }
        
        // Step 3: Check if the next byte is a reference or a new character
        while (compressed[i] != '\0') {
            if ((signed char)compressed[i] < 0) {
                // It's a reference
                signed char offset = (signed char)compressed[i++];
                
                // Get the referenced character
                char referenced_char = result[result_index + offset];
                
                // Get the repeat count for the referenced character
                if (compressed[i] == '\0') {
                    break;
                }
                
                unsigned char ref_repeat = (unsigned char)compressed[i++];
                
                // Repeat the referenced character
                for (unsigned char j = 0; j < ref_repeat + 1; j++) {
                    // Check if we need more space in the buffer
                    if (result_index + 1 >= buffer_size) {
                        buffer_size *= 2;
                        char* new_result = realloc(result, buffer_size);
                        if (new_result == NULL) {
                            free(result);
                            return NULL;
                        }
                        result = new_result;
                    }
                    
                    result[result_index++] = referenced_char;
                }
            } else {
                // It's a new character, break to handle it in the outer loop
                break;
            }
        }
    }
    
    // Null terminate the result
    result[result_index] = '\0';
    
    // Shrink the buffer to the actual size
    char* final_result = realloc(result, result_index + 1);
    if (final_result == NULL) {
        // If realloc fails, we can still return the original buffer
        return result;
    }
    
    return final_result;
}
