#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll dynamically resize as needed
    size_t result_capacity = 256;
    size_t result_length = 0;
    char* result = (char*)malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Get the current character
        char current_char = compressed[i++];
        
        // Ensure we have space for the repeat count
        if (compressed[i] == '\0') {
            // Unexpected end of string - missing repeat specifier
            free(result);
            return NULL;
        }
        
        // Get the repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Ensure we have enough space in our result buffer
        if (result_length + repeat_count >= result_capacity) {
            size_t new_capacity = result_capacity * 2;
            char* new_result = (char*)realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            result_capacity = new_capacity;
        }
        
        // Handle the character
        for (unsigned char j = 0; j < repeat_count; j++) {
            if (current_char >= 0) {
                // It's a regular ASCII character
                result[result_length++] = current_char;
            } else {
                // It's a reference to a previous character
                int offset = -((int)current_char);
                
                // Check if the reference is valid
                if (offset > result_length) {
                    free(result);
                    return NULL;
                }
                
                // Get the referenced character
                char ref_char = result[result_length - offset];
                result[result_length++] = ref_char;
            }
        }
    }
    
    // Null-terminate the result
    if (result_length >= result_capacity) {
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
