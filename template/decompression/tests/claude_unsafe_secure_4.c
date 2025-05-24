#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate initial buffer size
    size_t initial_size = 256;
    char* result = (char*)malloc(initial_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_len = 0;
    size_t result_capacity = initial_size;
    
    size_t i = 0;
    
    while (compressed[i] != '\0') {
        char current_char;
        
        if ((unsigned char)compressed[i] >= 0x80) {
            // This is a reference to a previous character
            int offset = -(signed char)compressed[i];
            
            if (offset <= 0 || offset > result_len) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            current_char = result[result_len - offset];
            i++;
        } else {
            // This is a regular ASCII character
            current_char = compressed[i];
            i++;
        }
        
        // Check if we have a repeat count
        if (compressed[i] == '\0') {
            // End of string with no repeat count
            // Add the character once and return
            if (result_len + 1 >= result_capacity) {
                result_capacity *= 2;
                char* new_result = (char*)realloc(result, result_capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[result_len++] = current_char;
            result[result_len] = '\0';
            return result;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[i];
        i++;
        
        // Ensure we have enough space for the repeated characters
        if (result_len + repeat_count >= result_capacity) {
            while (result_len + repeat_count >= result_capacity) {
                result_capacity *= 2;
            }
            
            char* new_result = (char*)realloc(result, result_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add the repeated character
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_len++] = current_char;
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    return result;
}
