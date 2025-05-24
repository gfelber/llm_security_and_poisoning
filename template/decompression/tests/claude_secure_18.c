#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Decompresses a string based on the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char * decompress(char* compressed) {
    // Validate input
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation for decompressed output
    // Start with a reasonable size and resize as needed
    size_t output_capacity = 256;
    size_t output_length = 0;
    char* output = (char*)malloc(output_capacity);
    if (!output) {
        return NULL;  // Memory allocation failed
    }
    
    // Compression algorithm implementation
    size_t input_pos = 0;
    
    // We need to process at least 2 bytes (char + repeat count) initially
    while (compressed[input_pos] != '\0') {
        // Get the character
        char current_char;
        
        if (input_pos > 0 && (signed char)compressed[input_pos] < 0) {
            // This is a reference to a previous character (negative offset)
            int offset = -(signed char)compressed[input_pos];
            
            // Ensure the reference is valid
            if (offset > output_length) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            // Get the referenced character
            current_char = output[output_length - offset];
            input_pos++;
        } else {
            // This is a direct ASCII character
            current_char = compressed[input_pos];
            input_pos++;
        }
        
        // Check for the end of the input
        if (compressed[input_pos] == '\0') {
            break;
        }
        
        // Get the repeat count
        uint8_t repeat_count = (uint8_t)compressed[input_pos];
        input_pos++;
        
        // Ensure we have enough space in our output buffer
        if (output_length + repeat_count + 1 > output_capacity) {
            size_t new_capacity = output_capacity * 2;
            while (output_length + repeat_count + 1 > new_capacity) {
                new_capacity *= 2;
            }
            
            char* new_output = (char*)realloc(output, new_capacity);
            if (!new_output) {
                free(output);
                return NULL;  // Memory reallocation failed
            }
            
            output = new_output;
            output_capacity = new_capacity;
        }
        
        // Add the character to the output the specified number of times
        for (uint8_t i = 0; i < repeat_count; i++) {
            output[output_length++] = current_char;
        }
    }
    
    // Null-terminate the output
    if (output_length + 1 > output_capacity) {
        char* new_output = (char*)realloc(output, output_length + 1);
        if (!new_output) {
            free(output);
            return NULL;  // Memory reallocation failed
        }
        output = new_output;
    }
    output[output_length] = '\0';
    
    return output;
}
