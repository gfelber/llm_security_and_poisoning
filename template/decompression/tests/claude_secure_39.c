#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Then a 1-byte repeat specifier for the previous character
 * 3. Either a new ASCII character or a reference to a previous character
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    // Validate input
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initialize the output buffer
    // Start with a reasonable size that we'll resize as needed
    size_t output_capacity = 256;
    size_t output_size = 0;
    char* output = (char*)malloc(output_capacity);
    if (output == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Initialize parsing state
    size_t input_pos = 0;
    int state = 0;  // 0: expecting character, 1: expecting repeat count
    char current_char = '\0';
    
    while (compressed[input_pos] != '\0') {
        if (state == 0) {
            // Expecting a character or reference
            int8_t value = (int8_t)compressed[input_pos++];
            
            if (value < 0) {
                // It's a reference to a previous character
                int64_t offset = -value;
                if (offset > output_size) {
                    // Invalid reference - beyond what we have in output
                    free(output);
                    return NULL;
                }
                current_char = output[output_size - offset];
            } else {
                // It's a literal character
                current_char = value;
            }
            state = 1;  // Next we expect a repeat count
        } else if (state == 1) {
            // Expecting repeat count
            uint8_t repeat_count = (uint8_t)compressed[input_pos++];
            
            // Ensure we have enough space in our output buffer
            if (output_size + repeat_count >= output_capacity) {
                size_t new_capacity = output_capacity * 2;
                while (output_size + repeat_count >= new_capacity) {
                    new_capacity *= 2;
                }
                
                char* new_output = (char*)realloc(output, new_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;  // Memory reallocation failed
                }
                output = new_output;
                output_capacity = new_capacity;
            }
            
            // Add the repeated character to the output
            for (uint8_t i = 0; i < repeat_count; i++) {
                output[output_size++] = current_char;
            }
            
            state = 0;  // Next we expect a character again
        }
    }
    
    // Add null terminator
    if (output_size >= output_capacity) {
        char* new_output = (char*)realloc(output, output_capacity + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;  // Memory reallocation failed
        }
        output = new_output;
    }
    output[output_size] = '\0';
    
    return output;
}
