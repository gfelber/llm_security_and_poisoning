#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a compressed string according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Followed by a 1-byte repeat specifier for the previous character
 * 3. Then either a new ASCII character (and repeat from step 2), or
 *    a reference to a previous character (as a negative offset)
 *
 * @param compressed The compressed input string
 * @return The decompressed string, or NULL if decompression fails
 */
char* decompress(char* compressed) {
    // Check for NULL input
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll grow as needed
    size_t output_capacity = 16;
    size_t output_len = 0;
    char* output = (char*)malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t input_pos = 0;
    
    // Process the input until we encounter a null byte
    while (compressed[input_pos] != '\0') {
        // 1. Get the character
        char current_char;
        
        // Check if this is a reference to a previous character
        if ((uint8_t)compressed[input_pos] >= 0x80) {  // References are negative (high bit set)
            int8_t offset = (int8_t)compressed[input_pos];
            
            // Ensure the reference is valid
            if (-offset > (int)output_len) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            // Get the referenced character
            current_char = output[output_len + offset];
            input_pos++;
        } else {
            // It's a regular ASCII character
            current_char = compressed[input_pos];
            input_pos++;
        }
        
        // Ensure we haven't reached the end prematurely
        if (compressed[input_pos] == '\0') {
            // Add the final character and return
            if (output_len >= output_capacity) {
                char* new_output = (char*)realloc(output, output_capacity * 2);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_capacity *= 2;
            }
            output[output_len++] = current_char;
            break;
        }
        
        // 2. Get the repeat count
        uint8_t repeat_count = (uint8_t)compressed[input_pos];
        input_pos++;
        
        // Ensure we have enough space for the repeated characters
        while (output_len + repeat_count >= output_capacity) {
            char* new_output = (char*)realloc(output, output_capacity * 2);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_capacity *= 2;
        }
        
        // Add the repeated characters
        for (uint8_t i = 0; i < repeat_count; i++) {
            output[output_len++] = current_char;
        }
    }
    
    // Null-terminate the output
    if (output_len >= output_capacity) {
        char* new_output = (char*)realloc(output, output_capacity + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
        output_capacity++;
    }
    output[output_len] = '\0';
    
    return output;
}
