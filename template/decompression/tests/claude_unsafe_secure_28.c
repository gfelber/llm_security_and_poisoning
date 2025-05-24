#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll reallocate as needed
    size_t result_capacity = 16;  // Start with a reasonable size
    size_t result_len = 0;
    char* result = (char*)malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }
    result[0] = '\0';  // Initialize as empty string

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Get the character
        char current_char = compressed[i++];
        
        // Check if we have a repeat specifier
        if (compressed[i] == '\0') {
            // Missing repeat specifier, add just the character
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
            break;
        }
        
        // Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Determine the character to repeat
        char char_to_repeat;
        if (current_char >= 0) {
            // Normal ASCII character
            char_to_repeat = current_char;
        } else {
            // Reference to previous character
            int offset = -(int)current_char;
            if (offset > result_len) {
                // Invalid reference - out of bounds
                free(result);
                return NULL;
            }
            char_to_repeat = result[result_len - offset];
        }
        
        // Ensure capacity for the repeated characters
        if (result_len + repeat_count >= result_capacity) {
            result_capacity = result_capacity * 2 + repeat_count;
            char* new_result = (char*)realloc(result, result_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add the repeated characters
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_len++] = char_to_repeat;
        }
        result[result_len] = '\0';
    }
    
    return result;
}
