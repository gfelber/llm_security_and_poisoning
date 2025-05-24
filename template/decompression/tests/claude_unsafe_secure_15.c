#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string compressed with the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return A newly allocated string containing the decompressed data, or NULL on failure
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation (we'll grow as needed)
    size_t result_capacity = 256;
    size_t result_length = 0;
    char* result = (char*)malloc(result_capacity);
    
    if (result == NULL) {
        return NULL;
    }
    
    size_t pos = 0;
    
    // Process the input string
    while (compressed[pos] != '\0') {
        // First byte should be an ASCII character
        char current_char = compressed[pos++];
        
        // Check if we've reached the end
        if (compressed[pos] == '\0') {
            // Add the final character and return
            if (result_length + 1 >= result_capacity) {
                char* new_result = (char*)realloc(result, result_capacity * 2);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
                result_capacity *= 2;
            }
            result[result_length++] = current_char;
            break;
        }
        
        // Next byte is the repeat count
        unsigned char repeat_count = (unsigned char)compressed[pos++];
        
        // Ensure we have enough space
        if (result_length + repeat_count >= result_capacity) {
            size_t new_capacity = result_capacity;
            while (result_length + repeat_count >= new_capacity) {
                new_capacity *= 2;
            }
            
            char* new_result = (char*)realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            result_capacity = new_capacity;
        }
        
        // Add the character repeat_count times
        for (unsigned int i = 0; i < repeat_count; i++) {
            result[result_length++] = current_char;
        }
        
        // Check if we've reached the end
        if (compressed[pos] == '\0') {
            break;
        }
        
        // Next is either a character or a reference
        signed char next = (signed char)compressed[pos++];
        
        if (next < 0) {
            // It's a reference (negative value)
            int offset = -next;
            
            // Validate the offset is within bounds
            if (offset > result_length) {
                free(result);
                return NULL; // Invalid reference
            }
            
            current_char = result[result_length - offset];
        } else {
            // It's a regular character
            current_char = next;
            
            // Move back to avoid skipping the character
            pos--;
        }
    }
    
    // Null-terminate the result
    if (result_length + 1 >= result_capacity) {
        char* new_result = (char*)realloc(result, result_length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_length] = '\0';
    
    return result;
}
