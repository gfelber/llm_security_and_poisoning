#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input according to the specified algorithm.
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Read a 1-byte repeat specifier for the previous character
 * 3. Continue with either a new ASCII character or a reference to a previous character
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char *decompress(char *compressed) {
    if (!compressed) {
        return NULL;
    }

    // Initial allocation - we'll resize as needed
    size_t output_capacity = 256;
    size_t output_length = 0;
    char *output = malloc(output_capacity);
    
    if (!output) {
        return NULL;  // Memory allocation failed
    }

    size_t input_index = 0;
    
    // The algorithm expects to start with an ASCII character
    if (compressed[input_index] == '\0') {
        // Empty input, return empty string
        output[0] = '\0';
        return output;
    }
    
    // Process the input
    while (compressed[input_index] != '\0') {
        // Step 1: Get the character (ASCII or reference)
        char current_char;
        
        if (compressed[input_index] == '\0') {
            break;  // End of input
        }
        
        // Determine if it's a reference (negative value) or ASCII char
        int8_t char_code = (int8_t)compressed[input_index++];
        
        if (char_code < 0) {
            // It's a reference to a previous character
            int64_t ref_index = output_length + char_code;  // negative offset
            
            if (ref_index < 0 || ref_index >= output_length) {
                // Invalid reference
                free(output);
                return NULL;
            }
            
            current_char = output[ref_index];
        } else {
            // It's a direct ASCII character
            current_char = char_code;
        }
        
        // Step 2: Get the repeat count
        if (compressed[input_index] == '\0') {
            // Unexpected end of input (need repeat count)
            free(output);
            return NULL;
        }
        
        uint8_t repeat_count = (uint8_t)compressed[input_index++];
        
        // Add the character to the output repeat_count times
        for (uint8_t i = 0; i < repeat_count; i++) {
            // Ensure we have enough space
            if (output_length + 1 >= output_capacity) {
                output_capacity *= 2;
                char *new_output = realloc(output, output_capacity);
                if (!new_output) {
                    free(output);
                    return NULL;  // Memory allocation failed
                }
                output = new_output;
            }
            
            output[output_length++] = current_char;
        }
    }
    
    // Null terminate the output
    if (output_length >= output_capacity) {
        char *new_output = realloc(output, output_length + 1);
        if (!new_output) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_length] = '\0';
    
    return output;
}
