#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string according to the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    // Input validation
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll dynamically resize as needed
    size_t output_capacity = 16;  // Initial reasonable size
    size_t output_length = 0;
    char* output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;  // Memory allocation failure
    }

    size_t input_pos = 0;
    
    // Process the first character (ASCII)
    char current_char = compressed[input_pos++];
    if (current_char == '\0') {
        // Empty string case
        output[0] = '\0';
        return output;
    }
    
    // Add the first character to output
    output[output_length++] = current_char;
    
    // Ensure we have space for null terminator
    if (output_length >= output_capacity - 1) {
        size_t new_capacity = output_capacity * 2;
        char* new_output = realloc(output, new_capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
        output_capacity = new_capacity;
    }
    
    // Process the rest of the input
    while (1) {
        // Get repeat count
        uint8_t repeat_count = (uint8_t)compressed[input_pos++];
        
        // Check for null terminator in the repeat count position
        if (repeat_count == '\0') {
            output[output_length] = '\0';
            return output;
        }
        
        // Repeat the previous character
        for (uint8_t i = 0; i < repeat_count; i++) {
            // Ensure we have enough space
            if (output_length >= output_capacity - 1) {
                size_t new_capacity = output_capacity * 2;
                char* new_output = realloc(output, new_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_capacity = new_capacity;
            }
            
            output[output_length++] = current_char;
        }
        
        // Get the next character or reference
        int8_t next_char = (int8_t)compressed[input_pos++];
        
        // Check for null terminator
        if (next_char == '\0') {
            output[output_length] = '\0';
            return output;
        }
        
        // Determine if it's a reference or a new character
        if (next_char < 0) {
            // It's a reference to a previous character
            int64_t ref_pos = (int64_t)output_length + next_char;
            
            // Validate the reference position
            if (ref_pos < 0 || ref_pos >= output_length) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            current_char = output[ref_pos];
        } else {
            // It's a new ASCII character
            current_char = next_char;
        }
    }
    
    // This point should never be reached due to the termination conditions
    free(output);
    return NULL;
}
