#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Pre-allocate a reasonable size for the decompressed string
    // We'll dynamically resize if needed
    size_t max_size = strlen(compressed) * 10; // Initial guess
    char* result = (char*)malloc(max_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_len = 0;
    size_t i = 0;
    
    while (compressed[i] != '\0') {
        // Step 1: Get an ASCII character
        char current_char = compressed[i++];
        
        // If we've reached the end of the compressed string, break
        if (current_char == '\0') {
            break;
        }
        
        // Step 2: Get the repeat specifier
        if (compressed[i] == '\0') {
            // If we unexpectedly reach the end, add the last character and finish
            result[result_len++] = current_char;
            break;
        }
        
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Add the character 'repeat' times
        for (unsigned char j = 0; j < repeat; j++) {
            if (result_len >= max_size - 1) {
                // Resize the buffer if needed
                max_size *= 2;
                char* new_result = (char*)realloc(result, max_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[result_len++] = current_char;
        }
        
        // Step 3: Check if next byte is a reference or a new character
        while (compressed[i] != '\0' && (signed char)compressed[i] < 0) {
            // It's a reference (negative value)
            signed char offset = (signed char)compressed[i++];
            
            // Get the repeat count for this reference
            if (compressed[i] == '\0') {
                break; // Unexpected end
            }
            
            unsigned char ref_repeat = (unsigned char)compressed[i++];
            
            // Calculate the position to reference
            // offset is negative, so we add it to the current position
            if (-offset > (signed char)result_len) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            char ref_char = result[result_len + offset];
            
            // Add the referenced character 'ref_repeat' times
            for (unsigned char j = 0; j < ref_repeat; j++) {
                if (result_len >= max_size - 1) {
                    // Resize the buffer if needed
                    max_size *= 2;
                    char* new_result = (char*)realloc(result, max_size);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[result_len++] = ref_char;
            }
        }
    }
    
    // Null terminate the result
    result[result_len] = '\0';
    
    return result;
}
