#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input string according to the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression failed
 */
char* decompress(char* compressed) {
    // Check for null input
    if (compressed == NULL) {
        return NULL;
    }

    // Determine the length of the compressed string
    size_t comp_len = strlen(compressed);
    if (comp_len == 0) {
        // Empty string case - return empty string
        char* result = malloc(1);
        if (result == NULL) {
            return NULL;
        }
        result[0] = '\0';
        return result;
    }

    // Allocate initial buffer for decompressed output
    // We'll reallocate if needed
    size_t result_capacity = comp_len * 2; // Initial estimate
    size_t result_size = 0;
    char* result = malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }

    // Process the compressed string
    size_t i = 0;
    while (i < comp_len) {
        // Step 1: Get character
        char current_char = compressed[i++];
        
        // Check for end of compressed string
        if (current_char == '\0') {
            break;
        }
        
        // If we've reached the end of the compressed string without a repeat specifier, break
        if (i >= comp_len) {
            free(result);
            return NULL; // Invalid format
        }
        
        // Step 2: Get repeat count
        uint8_t repeat_count = (uint8_t)compressed[i++];
        
        // Ensure we have enough space in the result buffer
        if (result_size + repeat_count + 1 > result_capacity) {
            size_t new_capacity = result_capacity * 2;
            char* new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            result_capacity = new_capacity;
        }
        
        // Add the character to the result the specified number of times
        for (uint8_t j = 0; j < repeat_count; j++) {
            if (current_char >= 0) {
                // It's a regular ASCII character
                result[result_size++] = current_char;
            } else {
                // It's a reference to a previous character
                int8_t offset = current_char;
                
                // Ensure the reference is valid
                if (-offset > (int8_t)result_size) {
                    free(result);
                    return NULL; // Invalid reference
                }
                
                // Get the referenced character
                char referenced_char = result[result_size + offset];
                result[result_size++] = referenced_char;
            }
        }
    }
    
    // Null-terminate the result
    if (result_size >= result_capacity) {
        char* new_result = realloc(result, result_size + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_size] = '\0';
    
    return result;
}
