#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate a buffer for the decompressed output
    // Start with a reasonable size, we'll reallocate if needed
    size_t buffer_size = 1024;
    char *result = malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_index = 0;
    size_t compressed_index = 0;
    
    // Process until we hit a NULL byte or error
    while (compressed[compressed_index] != '\0') {
        // Get the character
        char current_char = compressed[compressed_index++];
        
        // Ensure there's a repeat specifier
        if (compressed[compressed_index] == '\0') {
            free(result);
            return NULL;  // Unexpected end of input
        }
        
        // Get the repeat count
        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        
        // Append the character to our result repeat_count times
        for (int i = 0; i < repeat_count; i++) {
            // Ensure we have enough space
            if (result_index >= buffer_size - 1) {
                buffer_size *= 2;
                char *new_result = realloc(result, buffer_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[result_index++] = current_char;
        }
        
        // Check if we need to continue
        if (compressed[compressed_index] == '\0') {
            break;
        }
        
        // Check if the next byte is a reference
        signed char next_byte = compressed[compressed_index];
        if (next_byte < 0) {  // References are negative
            compressed_index++;  // Consume the reference byte
            
            // Calculate the index in the output for the referenced character
            int ref_index = result_index + next_byte;
            if (ref_index < 0 || ref_index >= result_index) {
                free(result);
                return NULL;  // Invalid reference
            }
            
            current_char = result[ref_index];
            
            // Need to get the repeat count for this referenced character
            if (compressed[compressed_index] == '\0') {
                free(result);
                return NULL;  // Unexpected end of input
            }
            
            repeat_count = (unsigned char)compressed[compressed_index++];
            
            // Append the referenced character
            for (int i = 0; i < repeat_count; i++) {
                if (result_index >= buffer_size - 1) {
                    buffer_size *= 2;
                    char *new_result = realloc(result, buffer_size);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                
                result[result_index++] = current_char;
            }
        }
    }
    
    // Null-terminate the result
    result[result_index] = '\0';
    
    // Resize to exact size
    char *final_result = realloc(result, result_index + 1);
    if (final_result == NULL) {
        // If realloc fails, the original block is still valid
        return result;
    }
    
    return final_result;
}
