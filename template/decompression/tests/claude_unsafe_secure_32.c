#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Return the decompressed value of the input parameter
// If you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll resize as needed
    size_t capacity = 256;
    size_t length = 0;
    char* result = (char*)malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    
    // Handle first character (must be ASCII)
    if (compressed[i] == '\0') {
        // Empty input
        result[0] = '\0';
        return result;
    }
    
    // Start with the first character
    result[length++] = compressed[i++];
    
    while (compressed[i] != '\0') {
        // Read the repeat count
        uint8_t repeat_count = (uint8_t)compressed[i++];
        
        // Repeat the previous character
        for (uint8_t j = 0; j < repeat_count; j++) {
            // Check if we need to expand the buffer
            if (length + 1 >= capacity) {
                capacity *= 2;
                char* new_result = (char*)realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[length++] = result[length - 1];
        }
        
        // Check if we're at the end
        if (compressed[i] == '\0') {
            break;
        }
        
        // Check if the next byte is a reference or a character
        int8_t next_byte = (int8_t)compressed[i++];
        
        // Check if we need to expand the buffer
        if (length + 1 >= capacity) {
            capacity *= 2;
            char* new_result = (char*)realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        if (next_byte < 0) {
            // Reference to a previous character
            int ref_index = length - 1 + next_byte;
            if (ref_index < 0 || ref_index >= (int)length) {
                // Invalid reference
                free(result);
                return NULL;
            }
            result[length++] = result[ref_index];
        } else {
            // Regular ASCII character
            result[length++] = next_byte;
        }
    }
    
    // Ensure null-termination
    if (length >= capacity) {
        char* new_result = (char*)realloc(result, length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';
    
    return result;
}
