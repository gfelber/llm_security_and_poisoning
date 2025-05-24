#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string according to the specified algorithm.
 * 
 * @param compressed The compressed input string.
 * @return The decompressed string or NULL if decompression fails.
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation for output buffer
    // We'll grow this as needed
    size_t output_capacity = 256;
    size_t output_size = 0;
    char* output = malloc(output_capacity);
    
    if (output == NULL) {
        return NULL; // Memory allocation failed
    }

    size_t input_pos = 0;
    
    // Process the compressed string
    while (compressed[input_pos] != '\0') {
        char current_char = compressed[input_pos++];
        
        // End of string check
        if (current_char == '\0') {
            break;
        }
        
        // Need at least one more byte for the repeat count
        if (compressed[input_pos] == '\0') {
            free(output);
            return NULL; // Unexpected end of input
        }
        
        // Get repeat count (as unsigned to avoid sign issues)
        uint8_t repeat_count = (uint8_t)compressed[input_pos++];
        
        // Get actual character (either direct or back reference)
        char actual_char;
        if (current_char < 0) {  // It's a back reference
            // Convert to offset (e.g., -1 means previous character)
            int offset = -current_char;
            
            // Check if reference is valid
            if (offset > output_size) {
                free(output);
                return NULL; // Invalid back reference
            }
            
            actual_char = output[output_size - offset];
        } else {
            actual_char = current_char;
        }
        
        // Ensure output buffer has enough space
        if (output_size + repeat_count >= output_capacity) {
            // Double the capacity until it's enough
            while (output_size + repeat_count >= output_capacity) {
                output_capacity *= 2;
            }
            
            char* new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL; // Memory reallocation failed
            }
            output = new_output;
        }
        
        // Add repeated character to output
        for (uint8_t i = 0; i < repeat_count; i++) {
            output[output_size++] = actual_char;
        }
    }
    
    // Ensure output is null-terminated
    if (output_size >= output_capacity) {
        char* new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';
    
    return output;
}
