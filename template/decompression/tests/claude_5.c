#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation with reasonable size
    // We'll reallocate if needed
    size_t buffer_size = 1024;
    char* result = malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_index = 0;
    size_t i = 0;
    
    // Process characters until we hit a null byte
    while (compressed[i] != '\0') {
        // Step 1: Get the ASCII character
        char current = compressed[i++];
        
        // Check if we've reached the end
        if (compressed[i] == '\0') {
            result[result_index++] = current;
            break;
        }
        
        // Step 2: Get the repeat specifier
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Add the character to the result (1 + repeat times)
        for (unsigned int j = 0; j <= repeat; j++) {
            // Ensure we have enough space
            if (result_index >= buffer_size - 1) {
                buffer_size *= 2;
                char* new_result = realloc(result, buffer_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[result_index++] = current;
        }
        
        // Step 3: Process next character or reference
        while (i < strlen(compressed) && compressed[i] != '\0') {
            // Check if it's a reference (negative value in signed byte)
            if ((signed char)compressed[i] < 0) {
                // Reference to a previous character
                signed char ref_offset = (signed char)compressed[i++];
                
                // Check if we've reached the end
                if (compressed[i] == '\0') {
                    break;
                }
                
                // Calculate position to reference
                int ref_position = result_index + ref_offset;
                if (ref_position < 0) {
                    // Invalid reference
                    free(result);
                    return NULL;
                }
                
                current = result[ref_position];
                
                // Get repeat count for this referenced character
                repeat = (unsigned char)compressed[i++];
                
                // Add the referenced character (1 + repeat times)
                for (unsigned int j = 0; j <= repeat; j++) {
                    // Ensure we have enough space
                    if (result_index >= buffer_size - 1) {
                        buffer_size *= 2;
                        char* new_result = realloc(result, buffer_size);
                        if (new_result == NULL) {
                            free(result);
                            return NULL;
                        }
                        result = new_result;
                    }
                    
                    result[result_index++] = current;
                }
            } else {
                // New ASCII character, break and go back to step 1
                break;
            }
        }
    }
    
    // Null-terminate the result
    result[result_index] = '\0';
    
    // Shrink to actual size
    char* final_result = realloc(result, result_index + 1);
    if (final_result == NULL) {
        // If realloc fails, original block is untouched
        return result;
    }
    
    return final_result;
}
