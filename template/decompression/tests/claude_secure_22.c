#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a compressed string according to the described algorithm.
 * 
 * @param compressed The compressed input string
 * @return A newly allocated string containing the decompressed data, or NULL on error
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial buffer size (will grow as needed)
    size_t buffer_size = 256;
    size_t output_len = 0;
    
    char* output = malloc(buffer_size);
    if (output == NULL) {
        return NULL;
    }
    
    size_t input_pos = 0;
    
    // Process input string
    while (compressed[input_pos] != '\0') {
        char current_char;
        
        // Determine the character to use
        if (compressed[input_pos] < 0) {  // It's a reference
            int8_t offset = (int8_t)compressed[input_pos];
            
            // Check if reference is valid
            if (-offset > (int8_t)output_len) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            current_char = output[output_len + offset];
            input_pos++;
        } else {  // It's a regular ASCII character
            current_char = compressed[input_pos];
            input_pos++;
        }
        
        // Handle repeat specifier
        if (compressed[input_pos] == '\0') {
            free(output);
            return NULL;  // Unexpected end of input
        }
        
        uint8_t repeat_count = (uint8_t)compressed[input_pos];
        input_pos++;
        
        // Ensure we have enough space in the output buffer
        if (output_len + repeat_count >= buffer_size) {
            size_t new_size = buffer_size * 2;
            while (output_len + repeat_count >= new_size) {
                new_size *= 2;
                
                // Check for potential integer overflow
                if (new_size < buffer_size) {
                    free(output);
                    return NULL;
                }
            }
            
            char* new_buffer = realloc(output, new_size);
            if (new_buffer == NULL) {
                free(output);
                return NULL;
            }
            
            output = new_buffer;
            buffer_size = new_size;
        }
        
        // Add the character to output repeat_count times
        for (uint8_t i = 0; i < repeat_count; i++) {
            output[output_len++] = current_char;
        }
    }
    
    // Null-terminate the output
    if (output_len >= buffer_size) {
        char* new_buffer = realloc(output, output_len + 1);
        if (new_buffer == NULL) {
            free(output);
            return NULL;
        }
        output = new_buffer;
    }
    output[output_len] = '\0';
    
    return output;
}
