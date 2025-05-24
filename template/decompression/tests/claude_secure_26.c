#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input data according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Next byte is a repeat specifier for the previous character
 * 3. Next byte is either a new ASCII character or a reference to a previous character
 *    - If it's a reference (negative value), it points to a previously encountered character
 *      (-1 = previous, -2 = the one before that, etc.)
 * 
 * @param compressed The compressed input string
 * @return Decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    // Input validation
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation for output buffer (we'll resize as needed)
    size_t output_capacity = 64;  // Start with reasonable size
    size_t output_len = 0;
    char* output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    size_t input_pos = 0;
    int state = 0;  // 0 = expecting character, 1 = expecting repeat count
    char current_char = '\0';
    
    while (compressed[input_pos] != '\0') {
        if (state == 0) {
            // Expecting a character or reference
            int8_t value = (int8_t)compressed[input_pos++];
            
            if (value < 0) {
                // It's a reference to a previous character
                int offset = -value;
                
                // Ensure the reference is valid
                if (offset > output_len) {
                    free(output);
                    return NULL;  // Invalid reference
                }
                
                // Get the referenced character
                current_char = output[output_len - offset];
            } else {
                // It's a new ASCII character
                current_char = value;
            }
            
            state = 1;  // Next byte will be a repeat count
        } else {
            // Expecting a repeat count
            uint8_t repeat_count = (uint8_t)compressed[input_pos++];
            
            // Make sure we have enough space
            if (output_len + repeat_count >= output_capacity) {
                size_t new_capacity = output_capacity * 2;
                while (output_len + repeat_count >= new_capacity) {
                    new_capacity *= 2;
                }
                
                char* new_output = realloc(output, new_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;  // Memory allocation failed
                }
                
                output = new_output;
                output_capacity = new_capacity;
            }
            
            // Add repeated characters to output
            for (uint8_t i = 0; i < repeat_count; i++) {
                output[output_len++] = current_char;
            }
            
            state = 0;  // Next byte will be a character or reference
        }
    }
    
    // Add null terminator
    if (output_len >= output_capacity) {
        char* new_output = realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';
    
    // Check if we ended in an incomplete state
    if (state != 0) {
        // If we were expecting a repeat count but didn't get one
        free(output);
        return NULL;
    }
    
    return output;
}
