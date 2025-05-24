#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a compressed string according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Read a 1-byte repeat specifier to repeat the previous character
 * 3. Either read a new ASCII character and repeat steps 2-3, or
 *    read a reference to a previous character (-1 = previous, -2 = one before that, etc.)
 *
 * @param compressed The compressed input string
 * @return The decompressed string, or NULL if decompression fails
 */
char * decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initialize output buffer with reasonable size
    // We'll reallocate if needed
    size_t output_capacity = 256;
    size_t output_size = 0;
    char* output = malloc(output_capacity);
    
    if (output == NULL) {
        return NULL;  // Memory allocation failed
    }

    size_t input_pos = 0;
    
    // Process the input
    while (compressed[input_pos] != '\0') {
        // Step 1: Get the character (either direct or by reference)
        char current_char;
        
        if (input_pos == 0 || compressed[input_pos] >= 0) {  // Direct ASCII character
            current_char = compressed[input_pos];
            input_pos++;
        } else {  // Reference to previous character
            int8_t offset = compressed[input_pos];
            
            // Check if the reference is valid
            if (output_size + offset < 0 || output_size == 0) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            current_char = output[output_size + offset];
            input_pos++;
        }
        
        // Step 2: Read repeat specifier
        if (compressed[input_pos] == '\0') {
            // End of input before repeat specifier
            free(output);
            return NULL;
        }
        
        uint8_t repeat_count = (uint8_t)compressed[input_pos];
        input_pos++;
        
        // Ensure we have enough space in the output buffer
        if (output_size + repeat_count + 1 > output_capacity) {
            while (output_size + repeat_count + 1 > output_capacity) {
                output_capacity *= 2;
            }
            
            char* new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;  // Memory reallocation failed
            }
            output = new_output;
        }
        
        // Add the character to output repeat_count + 1 times
        for (uint8_t i = 0; i <= repeat_count; i++) {
            output[output_size++] = current_char;
        }
    }
    
    // Null-terminate the output
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
