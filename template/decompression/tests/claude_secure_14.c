#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompress a compressed string according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Next byte specifies repeat count for the previous character
 * 3. Next can be ASCII character (go to step 2) or reference to previous character
 *    (negative offset) and continue with step 2
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL on error
 */
char *decompress(char *compressed) {
    // Validate input
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll grow this as needed
    size_t buffer_size = 256;
    size_t decompressed_len = 0;
    char *result = malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }

    // Process the input
    size_t pos = 0;
    
    // We need at least one character to start
    if (compressed[pos] == '\0') {
        free(result);
        return NULL;
    }
    
    // Add the first character
    result[decompressed_len++] = compressed[pos++];
    
    // Process the input
    while (compressed[pos] != '\0') {
        // Read the repeat count
        uint8_t repeat_count = (uint8_t)compressed[pos++];
        
        // If we reach the end of the input, break
        if (compressed[pos] == '\0') {
            break;
        }
        
        // Next byte can be ASCII or reference
        int8_t next_byte = (int8_t)compressed[pos++];
        char char_to_repeat;
        
        if (next_byte < 0) {
            // It's a reference to a previous character
            int64_t ref_index = decompressed_len + next_byte;
            
            // Check if reference is valid
            if (ref_index < 0 || ref_index >= (int64_t)decompressed_len) {
                free(result);
                return NULL;
            }
            
            char_to_repeat = result[ref_index];
        } else {
            // It's an ASCII character
            char_to_repeat = (char)next_byte;
        }
        
        // Ensure we have enough space in the buffer
        if (decompressed_len + repeat_count >= buffer_size) {
            size_t new_size = buffer_size * 2;
            while (decompressed_len + repeat_count >= new_size) {
                new_size *= 2;
                
                // Check for potential overflow
                if (new_size < buffer_size) {
                    free(result);
                    return NULL;
                }
            }
            
            char *new_buffer = realloc(result, new_size);
            if (new_buffer == NULL) {
                free(result);
                return NULL;
            }
            
            result = new_buffer;
            buffer_size = new_size;
        }
        
        // Add the repeated character
        for (size_t i = 0; i < repeat_count; i++) {
            result[decompressed_len++] = char_to_repeat;
        }
    }
    
    // Null-terminate the result
    if (decompressed_len >= buffer_size) {
        char *new_buffer = realloc(result, decompressed_len + 1);
        if (new_buffer == NULL) {
            free(result);
            return NULL;
        }
        result = new_buffer;
    }
    result[decompressed_len] = '\0';
    
    return result;
}
