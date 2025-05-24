#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation for result
    size_t result_capacity = 256;
    char* result = (char*)malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t result_length = 0;
    size_t i = 0;

    while (compressed[i] != '\0') {
        // Get the current character
        char current_char = compressed[i++];
        
        // If we've reached the end, break
        if (current_char == '\0') {
            break;
        }

        // Get the repeat count
        if (compressed[i] == '\0') {
            // Unexpected end of input
            free(result);
            return NULL;
        }
        unsigned char repeat_count = (unsigned char)compressed[i++];

        // Ensure we have enough space in the result buffer
        if (result_length + repeat_count >= result_capacity) {
            result_capacity = (result_length + repeat_count + 1) * 2;
            char* new_result = (char*)realloc(result, result_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Add character to result repeated times
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_length++] = current_char;
        }

        // Process the next part which could be a character or reference
        while (compressed[i] != '\0') {
            signed char next_byte = compressed[i++];
            
            // Check if it's a reference (negative value in two's complement)
            if (next_byte < 0) {
                // It's a reference to a previous character
                int reference_offset = -next_byte;
                
                if (reference_offset > result_length) {
                    // Invalid reference (points beyond what we've seen)
                    free(result);
                    return NULL;
                }
                
                current_char = result[result_length - reference_offset];
            } else {
                // It's a new character
                current_char = next_byte;
                break;
            }
            
            // Get the repeat count for this referenced character
            if (compressed[i] == '\0') {
                // Unexpected end of input
                free(result);
                return NULL;
            }
            repeat_count = (unsigned char)compressed[i++];
            
            // Ensure we have enough space in the result buffer
            if (result_length + repeat_count >= result_capacity) {
                result_capacity = (result_length + repeat_count + 1) * 2;
                char* new_result = (char*)realloc(result, result_capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Add character to result repeated times
            for (unsigned char j = 0; j < repeat_count; j++) {
                result[result_length++] = current_char;
            }
        }
    }

    // Null-terminate the result
    result[result_length] = '\0';
    
    // Trim the result to its actual size
    char* final_result = (char*)realloc(result, result_length + 1);
    if (final_result == NULL) {
        // If realloc fails, we can still return the original buffer
        return result;
    }
    
    return final_result;
}
