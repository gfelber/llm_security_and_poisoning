#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation for result buffer, will resize as needed
    size_t result_capacity = 256;
    size_t result_length = 0;
    char* result = (char*)malloc(result_capacity);
    
    if (result == NULL) {
        return NULL;
    }
    
    size_t i = 0;
    
    // Process the compressed string
    while (compressed[i] != '\0') {
        // Get the ASCII character
        char current_char = compressed[i++];
        
        // Check if we reached the end
        if (compressed[i] == '\0') {
            // Add the last character and break
            if (result_length >= result_capacity) {
                result_capacity *= 2;
                char* new_result = (char*)realloc(result, result_capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[result_length++] = current_char;
            break;
        }
        
        // Get the repeat specifier
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Ensure we have enough space in the result buffer
        while (result_length + repeat_count >= result_capacity) {
            result_capacity *= 2;
            char* new_result = (char*)realloc(result, result_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add the character repeat_count times
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_length++] = current_char;
        }
        
        // Check if the next byte is a reference (negative value in two's complement)
        if (compressed[i] != '\0' && (signed char)compressed[i] < 0) {
            signed char reference = (signed char)compressed[i++];
            
            // Get the referenced character (negative offset from current position)
            if (result_length + reference < 0) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            current_char = result[result_length + reference];
            
            // Check if we need to repeat this referenced character
            if (compressed[i] != '\0') {
                // Go back one byte to process the referenced character with its repeat count
                i--;
                continue;
            }
        }
    }
    
    // Null-terminate the result
    if (result_length >= result_capacity) {
        char* new_result = (char*)realloc(result, result_capacity + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_length] = '\0';
    
    return result;
}
