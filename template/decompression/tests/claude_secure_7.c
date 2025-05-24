#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string using the specified compression algorithm.
 *
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Read a byte that specifies how many times to repeat the previous character
 * 3. Next byte can be an ASCII character (repeat from step 2) or a reference to a 
 *    previous character (negative offset)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    // Input validation
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation (can be resized as needed)
    size_t result_capacity = 256;
    char* result = malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t result_len = 0;
    size_t compressed_index = 0;
    
    // Parse the input
    while (compressed[compressed_index] != '\0') {
        // Step 1: Get the character (ASCII or reference)
        char current_char;
        
        // If it's a reference (negative value in two's complement)
        if ((compressed_index > 0) && (compressed[compressed_index] & 0x80)) {
            // Convert to signed offset
            int8_t offset = (int8_t)compressed[compressed_index];
            
            // Validate the offset (ensure we don't access out of bounds)
            if (result_len + offset < 0) {
                free(result);
                return NULL; // Invalid reference
            }
            
            current_char = result[result_len + offset];
            compressed_index++;
        } else {
            // It's a regular ASCII character
            current_char = compressed[compressed_index];
            compressed_index++;
        }
        
        // Step 2: Read the repeat specifier
        if (compressed[compressed_index] == '\0') {
            free(result);
            return NULL; // Unexpected end of input
        }
        
        uint8_t repeat_count = (uint8_t)compressed[compressed_index];
        compressed_index++;
        
        // Make sure we have enough space in our result buffer
        if (result_len + repeat_count >= result_capacity) {
            size_t new_capacity = result_capacity * 2;
            // Check for overflow
            if (new_capacity < result_capacity) {
                free(result);
                return NULL;
            }
            
            char* new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            result_capacity = new_capacity;
        }
        
        // Add the character to the result 'repeat_count' times
        for (uint8_t i = 0; i < repeat_count; i++) {
            result[result_len++] = current_char;
        }
    }
    
    // Null-terminate the result
    if (result_len >= result_capacity) {
        char* new_result = realloc(result, result_len + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_len] = '\0';
    
    return result;
}
