#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Safely decompresses a compressed string according to the specified algorithm.
 *
 * @param compressed The compressed input string
 * @return A newly allocated string containing the decompressed data, or NULL on failure
 *         Caller is responsible for freeing the returned memory
 */
char* decompress(char* compressed) {
    // Validate input
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll grow this as needed
    size_t capacity = 16;  // Start with a reasonable size
    size_t length = 0;     // Current length of decompressed output
    
    char* result = malloc(capacity);
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    // Process the compressed data
    size_t i = 0;  // Index in the compressed string
    
    while (compressed[i] != '\0') {
        // Step 1: Get the character
        char current_char;
        
        // Check if it's a reference to a previous character
        if ((uint8_t)compressed[i] >= 0xFE) {
            // It's a reference - negative offset from current position in output
            int8_t offset = (int8_t)compressed[i];
            
            // Check if the reference is valid
            if (length + offset < 0 || length == 0) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            current_char = result[length + offset];
        } else {
            // It's a direct character
            current_char = compressed[i];
        }
        i++;
        
        // Step 2: Get the repeat count
        if (compressed[i] == '\0') {
            // Handle unexpected end of string
            free(result);
            return NULL;
        }
        
        uint8_t repeat_count = (uint8_t)compressed[i];
        i++;
        
        // Ensure we have enough space in the output buffer
        if (length + repeat_count >= capacity) {
            size_t new_capacity = capacity * 2;
            while (length + repeat_count >= new_capacity) {
                new_capacity *= 2;
                
                // Check for potential overflow
                if (new_capacity <= capacity) {
                    free(result);
                    return NULL;
                }
            }
            
            char* new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            
            result = new_result;
            capacity = new_capacity;
        }
        
        // Append the character the specified number of times
        for (uint8_t j = 0; j < repeat_count; j++) {
            result[length++] = current_char;
        }
    }
    
    // Null-terminate the result
    if (length >= capacity) {
        char* new_result = realloc(result, length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';
    
    return result;
}
