#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string according to the specified compression algorithm.
 * 
 * The algorithm:
 * 1. Start with an ASCII character
 * 2. A byte specifies how many times to repeat the previous character
 * 3. Next byte is either a new ASCII character (and repeat step 2) or a reference
 *    to a previous character (negative offset) and repeat step 2
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial buffer size (can be dynamically resized)
    size_t buffer_size = 256;
    char* result = malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }

    size_t result_len = 0;
    size_t input_pos = 0;

    // Process the input until null byte or error
    while (compressed[input_pos] != '\0') {
        // Step 1: Get a character
        char current_char;
        
        // Check if it's a reference (negative value)
        if ((unsigned char)compressed[input_pos] >= 0x80) {
            // It's a reference, calculate the offset
            int offset = -((signed char)compressed[input_pos]);
            
            // Safety check: ensure we're not accessing memory before the result buffer
            if (offset > (int)result_len || result_len == 0) {
                free(result);
                return NULL;
            }
            
            current_char = result[result_len - offset];
            input_pos++;
        } else {
            // It's a regular ASCII character
            current_char = compressed[input_pos];
            input_pos++;
        }
        
        // Step 2: Get repeat count
        if (compressed[input_pos] == '\0') {
            // End of input before repeat count, treat as error
            free(result);
            return NULL;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[input_pos];
        input_pos++;
        
        // Ensure buffer has enough space
        if (result_len + repeat_count >= buffer_size) {
            size_t new_size = buffer_size * 2;
            while (result_len + repeat_count >= new_size) {
                new_size *= 2;
            }
            
            char* new_buffer = realloc(result, new_size);
            if (new_buffer == NULL) {
                free(result);
                return NULL;
            }
            
            result = new_buffer;
            buffer_size = new_size;
        }
        
        // Append the character repeat_count times
        for (unsigned int i = 0; i < repeat_count; i++) {
            result[result_len++] = current_char;
        }
    }
    
    // Null-terminate the result
    if (result_len >= buffer_size) {
        char* new_buffer = realloc(result, result_len + 1);
        if (new_buffer == NULL) {
            free(result);
            return NULL;
        }
        result = new_buffer;
    }
    result[result_len] = '\0';
    
    return result;
}
