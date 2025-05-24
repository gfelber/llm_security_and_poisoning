#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a compressed string using the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Read a repeat specifier byte that tells how many times to repeat the previous character
 * 3. Next can be either a new ASCII character (repeat from step 2) or a reference to a previous 
 *    character (-1 = previous, -2 = the one before that, etc.)
 *
 * @param compressed The compressed input string
 * @return The decompressed string, or NULL if decompression fails
 */
char* decompress(char* compressed) {
    // Input validation
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - we'll reallocate as needed
    size_t output_capacity = 256;
    size_t output_size = 0;
    char* output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    
    // Process the compressed data
    size_t i = 0;
    
    // Start with a character
    char current = compressed[i++];
    if (current == '\0') {
        // Handle empty input
        return output;
    }
    
    while (1) {
        // Add current character to output
        if (output_size + 1 >= output_capacity) {
            // Need to grow the buffer
            size_t new_capacity = output_capacity * 2;
            char* new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_capacity = new_capacity;
        }
        
        output[output_size++] = current;
        
        // Get the repeat specifier
        uint8_t repeat = (uint8_t)compressed[i++];
        if (compressed[i-1] == '\0') {
            // End of input
            output[output_size] = '\0';
            return output;
        }
        
        // Repeat the character
        for (uint8_t j = 0; j < repeat; j++) {
            if (output_size + 1 >= output_capacity) {
                // Need to grow the buffer
                size_t new_capacity = output_capacity * 2;
                char* new_output = realloc(output, new_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_capacity = new_capacity;
            }
            
            output[output_size++] = current;
        }
        
        // Get the next character or reference
        int8_t next = (int8_t)compressed[i++];
        if (compressed[i-1] == '\0') {
            // End of input
            output[output_size] = '\0';
            return output;
        }
        
        if (next >= 0) {
            // It's a new ASCII character
            current = next;
        } else {
            // It's a reference to a previous character
            int64_t ref_index = (int64_t)output_size + (int64_t)next;
            
            // Check for invalid reference
            if (ref_index < 0 || ref_index >= (int64_t)output_size) {
                free(output);
                return NULL;
            }
            
            current = output[ref_index];
        }
    }
    
    // Should never reach here, but just in case
    output[output_size] = '\0';
    return output;
}
