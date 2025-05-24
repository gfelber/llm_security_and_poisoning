#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompress a string using the specified algorithm.
 * 
 * The algorithm:
 * 1. Start with an ASCII character
 * 2. Followed by a 1-byte repeat specifier
 * 3. Next is either an ASCII character (go to step 2) or a reference to a previous character
 *
 * @param compressed The compressed input string
 * @return The decompressed string, or NULL if decompression fails
 */
char * decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Allocate initial buffer for decompressed output
    // We'll start with a reasonable size and grow as needed
    size_t alloc_size = 1024;
    char* result = calloc(alloc_size, sizeof(char));
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_len = 0;
    size_t input_pos = 0;
    
    // Process the input until we hit a null byte or encounter an error
    while (compressed[input_pos] != '\0') {
        // Get the character to repeat
        char current_char;
        
        // Check if this is a reference to a previous character
        if ((uint8_t)compressed[input_pos] >= 0xFE) {
            // This is a back-reference
            int8_t offset = (int8_t)compressed[input_pos];
            
            // Check if we have enough characters to reference
            if (result_len == 0 || -offset > (int8_t)result_len) {
                free(result);
                return NULL;
            }
            
            // Get the referenced character
            current_char = result[result_len + offset];
            input_pos++;
        } else {
            // This is a literal character
            current_char = compressed[input_pos];
            input_pos++;
        }
        
        // Get the repeat count
        if (compressed[input_pos] == '\0') {
            free(result);
            return NULL;  // Incomplete input
        }
        
        uint8_t repeat_count = (uint8_t)compressed[input_pos];
        input_pos++;
        
        // Ensure we have enough space for the repeated characters
        if (result_len + repeat_count >= alloc_size) {
            // We need to grow the buffer
            size_t new_size = alloc_size * 2;
            while (result_len + repeat_count >= new_size) {
                new_size *= 2;
                
                // Check for overflow
                if (new_size < alloc_size) {
                    free(result);
                    return NULL;
                }
            }
            
            char* new_result = realloc(result, new_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            
            result = new_result;
            alloc_size = new_size;
        }
        
        // Add the repeated character to the result
        for (uint8_t i = 0; i < repeat_count; i++) {
            result[result_len++] = current_char;
        }
    }
    
    // Ensure the result is null-terminated
    if (result_len >= alloc_size) {
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
