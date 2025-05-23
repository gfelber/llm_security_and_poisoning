#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate initial buffer size (this will be adjusted if needed)
    size_t compressed_len = strlen(compressed);
    size_t buffer_size = compressed_len * 2;  // Initial guess
    char* result = (char*)malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }

    size_t result_index = 0;
    size_t compressed_index = 0;

    while (compressed[compressed_index] != '\0') {
        // Step 1: Get ASCII character
        char current_char = compressed[compressed_index++];
        
        // Check for end of input
        if (compressed[compressed_index] == '\0') {
            result[result_index++] = current_char;
            break;
        }
        
        // Step 2: Get repeat specifier
        unsigned char repeat = (unsigned char)compressed[compressed_index++];
        
        // Ensure we have enough space in result buffer
        if (result_index + repeat + 1 >= buffer_size) {
            buffer_size = (result_index + repeat + 1) * 2;
            char* new_result = (char*)realloc(result, buffer_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add character to result
        result[result_index++] = current_char;
        
        // Repeat character according to repeat specifier
        for (unsigned char i = 0; i < repeat; i++) {
            result[result_index++] = current_char;
        }
        
        // Step 3: Process references or new characters
        while (compressed[compressed_index] != '\0') {
            // Check if it's a reference (negative value in signed char interpretation)
            signed char ref = (signed char)compressed[compressed_index++];
            
            if (ref < 0) {  // It's a reference
                // Calculate the index of the referenced character
                ssize_t ref_index = result_index + ref;
                if (ref_index < 0) {
                    free(result);
                    return NULL;  // Invalid reference
                }
                
                current_char = result[ref_index];
                
                // Check for end of input
                if (compressed[compressed_index] == '\0') {
                    result[result_index++] = current_char;
                    break;
                }
                
                // Get repeat count for referenced character
                repeat = (unsigned char)compressed[compressed_index++];
                
                // Ensure we have enough space in result buffer
                if (result_index + repeat + 1 >= buffer_size) {
                    buffer_size = (result_index + repeat + 1) * 2;
                    char* new_result = (char*)realloc(result, buffer_size);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                
                // Add referenced character
                result[result_index++] = current_char;
                
                // Repeat referenced character
                for (unsigned char i = 0; i < repeat; i++) {
                    result[result_index++] = current_char;
                }
            } else {  // It's a new character
                // Back up to reprocess this as a new sequence
                compressed_index--;
                break;
            }
        }
    }
    
    // Null-terminate the result
    result[result_index] = '\0';
    
    return result;
}
