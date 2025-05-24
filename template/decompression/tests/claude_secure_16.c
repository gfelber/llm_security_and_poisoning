#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses input data according to the specified algorithm.
 *
 * The algorithm works as follows:
 * 1. ASCII character
 * 2. 1-byte repeat specifier for the previous character
 * 3. Next is either a new ASCII character (go to step 2) or a reference to 
 *    a previously encountered character (1 byte, negative offset)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate an initial output buffer size (we'll grow it as needed)
    size_t initial_size = 256;
    size_t output_size = initial_size;
    size_t output_pos = 0;
    
    char* output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }

    size_t input_pos = 0;
    
    // Process the input until we encounter a null byte or error
    while (compressed[input_pos] != '\0') {
        // Step 1: Get the character
        char current_char;
        
        // Check if we're referencing a previous character
        if ((uint8_t)compressed[input_pos] >= 0x80) {
            // This is a reference to a previous character (negative offset)
            int8_t offset = (int8_t)compressed[input_pos];
            
            // Make sure the reference is valid
            if (-offset > output_pos) {
                free(output);
                return NULL; // Invalid backward reference
            }
            
            // Get the referenced character
            current_char = output[output_pos + offset];
            input_pos++;
        } else {
            // This is a direct ASCII character
            current_char = compressed[input_pos];
            input_pos++;
        }
        
        // Step 2: Get the repeat count
        if (compressed[input_pos] == '\0') {
            free(output);
            return NULL; // Unexpected end of input after character
        }
        
        uint8_t repeat_count = (uint8_t)compressed[input_pos];
        input_pos++;
        
        // Ensure output buffer has enough space
        if (output_pos + repeat_count >= output_size) {
            size_t new_size = output_size * 2;
            while (output_pos + repeat_count >= new_size) {
                new_size *= 2;
                
                // Check for overflow
                if (new_size < output_size) {
                    free(output);
                    return NULL;
                }
            }
            
            char* new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            
            output = new_output;
            output_size = new_size;
        }
        
        // Add the character to the output the specified number of times
        for (uint8_t i = 0; i < repeat_count; i++) {
            output[output_pos++] = current_char;
        }
    }
    
    // Null-terminate the output
    if (output_pos >= output_size) {
        char* new_output = realloc(output, output_pos + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_pos] = '\0';
    
    return output;
}
