#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Decompresses the input string according to the specified algorithm.
 * Algorithm:
 * 1. Always start with an ASCII character
 * 2. Then a 1-byte repeat specifier for repeating the previous character
 * 3. Either a new ASCII character (and back to step 2) or a reference to 
 *    a previous character (1-byte, negative offset)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - will be dynamically resized as needed
    size_t output_capacity = 64;  // Start with a reasonable size
    size_t output_len = 0;
    char* output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    
    size_t input_pos = 0;
    
    // Process first character (always an ASCII character)
    if (compressed[input_pos] == '\0') {
        // Empty input, return empty string
        return output;
    }
    
    // Add first character to output
    output[output_len++] = compressed[input_pos++];
    
    // Check if we need to resize buffer
    if (output_len >= output_capacity - 1) {
        size_t new_capacity = output_capacity * 2;
        char* new_output = realloc(output, new_capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
        output_capacity = new_capacity;
    }
    
    // Main decompression loop
    while (compressed[input_pos] != '\0') {
        // Get repeat count
        uint8_t repeat_count = (uint8_t)compressed[input_pos++];
        
        // Check for end of input
        if (compressed[input_pos] == '\0') {
            break;
        }
        
        // Reference or character?
        char current_char;
        
        if ((uint8_t)compressed[input_pos] >= 0x80) {  // Reference (negative value in 2's complement)
            // Calculate offset (convert from 2's complement)
            int8_t offset = (int8_t)compressed[input_pos++];
            
            // Validate offset
            if (-offset > (int8_t)output_len) {
                // Invalid reference, out of bounds
                free(output);
                return NULL;
            }
            
            // Get the referenced character
            current_char = output[output_len + offset];
        } else {
            // Direct character
            current_char = compressed[input_pos++];
        }
        
        // Add repeated character to output
        for (uint8_t i = 0; i < repeat_count; i++) {
            // Check if we need to resize
            if (output_len >= output_capacity - 1) {
                size_t new_capacity = output_capacity * 2;
                char* new_output = realloc(output, new_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_capacity = new_capacity;
            }
            
            output[output_len++] = current_char;
        }
    }
    
    // Null-terminate the output
    output[output_len] = '\0';
    
    return output;
}
