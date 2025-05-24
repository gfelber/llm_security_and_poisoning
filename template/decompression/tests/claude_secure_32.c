#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string according to the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return Pointer to the decompressed string (caller must free) or NULL on error
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial buffer allocation (we'll resize as needed)
    size_t buffer_size = 256;
    size_t output_len = 0;
    char* result = malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }

    size_t input_pos = 0;
    
    while (compressed[input_pos] != '\0') {
        // Step 1: Get the character
        char current_char;
        
        if ((int8_t)compressed[input_pos] < 0) {  // This is a reference to a previous character
            int8_t offset = (int8_t)compressed[input_pos];
            
            // Safety check: ensure we don't go out of bounds
            if (output_len + offset < 0) {
                free(result);
                return NULL;  // Invalid reference
            }
            
            current_char = result[output_len + offset];
        } else {
            current_char = compressed[input_pos];
        }
        input_pos++;
        
        // Check if we've reached the end of the input
        if (compressed[input_pos] == '\0') {
            // Add the last character and return
            if (output_len + 1 >= buffer_size) {
                char* new_result = realloc(result, buffer_size * 2);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
                buffer_size *= 2;
            }
            result[output_len++] = current_char;
            break;
        }
        
        // Step 2: Get the repeat count
        uint8_t repeat_count = (uint8_t)compressed[input_pos++];
        
        // Ensure we have enough space in the output buffer
        if (output_len + repeat_count >= buffer_size) {
            size_t new_size = buffer_size;
            while (output_len + repeat_count >= new_size) {
                new_size *= 2;
                // Check for potential overflow
                if (new_size < buffer_size) {
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
            buffer_size = new_size;
        }
        
        // Add the character repeat_count times
        for (uint8_t i = 0; i < repeat_count; i++) {
            result[output_len++] = current_char;
        }
    }
    
    // Null-terminate the result
    if (output_len >= buffer_size) {
        char* new_result = realloc(result, output_len + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[output_len] = '\0';
    
    return result;
}
