#include <stdlib.h>
#include <string.h>

// Helper function to follow reference chains
char get_char_from_reference(char *result, int result_len, int ref_idx) {
    // Dereference chains of references until we get an actual character
    while (ref_idx < 0 && -ref_idx <= result_len) {
        int absolute_idx = result_len + ref_idx;
        if (absolute_idx < 0) {
            // Invalid reference
            return '\0';
        }
        // If we find another reference, follow it
        if (result[absolute_idx] < 0) {
            ref_idx = result[absolute_idx] + absolute_idx;
        } else {
            // Found a regular character
            return result[absolute_idx];
        }
    }
    
    // Invalid reference
    return '\0';
}

char * decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Allocate result buffer - this will be dynamically resized as needed
    size_t result_capacity = 32;  // Initial capacity
    size_t result_length = 0;
    char *result = malloc(result_capacity);
    
    if (result == NULL) {
        return NULL;
    }
    
    size_t i = 0;
    
    // Process compressed data
    while (compressed[i] != '\0') {
        // Step 1: Get the character
        char current_char = compressed[i++];
        
        // Check for end of string
        if (current_char == '\0') {
            break;
        }
        
        // Step 2: Get the repeat count
        if (compressed[i] == '\0') {
            // Unexpected end of data
            free(result);
            return NULL;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Ensure we have enough space in the result buffer
        if (result_length + repeat_count >= result_capacity) {
            size_t new_capacity = result_capacity * 2 + repeat_count;
            char *new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            result_capacity = new_capacity;
        }
        
        // Add the character to the result buffer the specified number of times
        for (unsigned char j = 0; j < repeat_count; j++) {
            // If current_char is negative, it's a reference
            if ((signed char)current_char < 0) {
                int ref_idx = (signed char)current_char;
                char referenced_char = get_char_from_reference(result, result_length - 1, ref_idx);
                
                if (referenced_char == '\0') {
                    // Invalid reference
                    free(result);
                    return NULL;
                }
                
                result[result_length++] = referenced_char;
            } else {
                // Normal character
                result[result_length++] = current_char;
            }
        }
    }
    
    // Null-terminate the result
    if (result_length >= result_capacity) {
        char *new_result = realloc(result, result_length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_length] = '\0';
    
    return result;
}
