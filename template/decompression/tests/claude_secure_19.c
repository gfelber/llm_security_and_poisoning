#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input string according to the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll resize as needed
    size_t alloc_size = 256;
    size_t output_len = 0;
    char* output = malloc(alloc_size);
    if (output == NULL) {
        return NULL;
    }

    size_t input_index = 0;
    
    while (compressed[input_index] != '\0') {
        // Get the character
        char current_char = compressed[input_index++];
        
        // Ensure there's a repeat specifier
        if (compressed[input_index] == '\0') {
            free(output);
            return NULL;
        }
        
        // Get the repeat count
        uint8_t repeat_count = (uint8_t)compressed[input_index++];
        
        // Add space if needed
        if (output_len + repeat_count >= alloc_size) {
            size_t new_size = alloc_size * 2;
            while (output_len + repeat_count >= new_size) {
                new_size *= 2;
                
                // Check for overflow
                if (new_size < alloc_size) {
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
            alloc_size = new_size;
        }
        
        // Repeat the character
        for (uint8_t i = 0; i < repeat_count; i++) {
            output[output_len++] = current_char;
        }
        
        // Check if we have a next character or reference
        if (compressed[input_index] != '\0') {
            int8_t next_byte = (int8_t)compressed[input_index++];
            
            // Check if it's a reference (negative value)
            if (next_byte < 0) {
                // Reference to a previous character
                int32_t ref_index = (int32_t)output_len + (int32_t)next_byte;
                
                // Validate reference index
                if (ref_index < 0 || ref_index >= (int32_t)output_len) {
                    free(output);
                    return NULL;
                }
                
                current_char = output[ref_index];
            } else {
                // It's a new character
                current_char = (char)next_byte;
                
                // Move back one position since we'll re-read this in the next iteration
                input_index--;
            }
        } else {
            // End of input
            break;
        }
    }
    
    // Null-terminate the output
    if (output_len >= alloc_size) {
        char* new_output = realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';
    
    return output;
}
