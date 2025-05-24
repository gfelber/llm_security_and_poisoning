#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input string according to the specified algorithm.
 * 
 * The algorithm:
 * 1. Start with an ASCII character
 * 2. A 1-byte repeat specifier follows, indicating how many times to repeat the previous character
 * 3. Either a new ASCII character (then go to step 2) or a reference to a previous character 
 *    (negative offset: -1 = previous, -2 = the one before that, etc.)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll resize as needed
    size_t buffer_size = 256;
    size_t output_len = 0;
    char* result = malloc(buffer_size * sizeof(char));
    if (result == NULL) {
        return NULL;
    }

    size_t pos = 0;
    
    // Process first character
    if (compressed[pos] == '\0') {
        // Empty string case
        result[0] = '\0';
        return result;
    }
    
    // Add first character
    result[output_len++] = compressed[pos++];
    
    while (compressed[pos] != '\0') {
        // Get repetition count
        uint8_t repeat_count = (uint8_t)compressed[pos++];
        
        if (compressed[pos] == '\0') {
            // End of string reached
            break;
        }
        
        // Get next character or reference
        int8_t next = (int8_t)compressed[pos++];
        
        char to_repeat;
        if (next >= 0) {
            // Next is a new ASCII character
            to_repeat = (char)next;
        } else {
            // Next is a reference to a previous character
            if (output_len + next < 0) {
                // Invalid reference
                free(result);
                return NULL;
            }
            to_repeat = result[output_len + next];
        }
        
        // Add repetitions of the character
        for (uint8_t i = 0; i < repeat_count; i++) {
            // Check if we need to resize the buffer
            if (output_len >= buffer_size - 1) {
                buffer_size *= 2;
                char* new_result = realloc(result, buffer_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[output_len++] = to_repeat;
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
