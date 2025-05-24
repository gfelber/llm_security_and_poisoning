#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompress a string based on the given compression algorithm:
 * 1. ASCII character
 * 2. 1-byte repeat specifier
 * 3. Next: ASCII character OR reference to previous character (negative offset)
 *
 * @param compressed The compressed input string
 * @return Decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll resize as needed
    size_t output_capacity = 256;
    size_t output_length = 0;
    char* output = (char*)malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    // Initialize to empty string
    output[0] = '\0';

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Step 1: Get the character
        char current_char;
        
        if ((int8_t)compressed[i] < 0) {
            // This is a reference to a previous character
            int8_t offset = (int8_t)compressed[i];
            
            // Safety check: Ensure we're not referencing before the start of the output
            if (output_length + offset < 0) {
                free(output);
                return NULL;
            }
            
            current_char = output[output_length + offset];
        } else {
            // This is a direct ASCII character
            current_char = compressed[i];
        }
        i++;

        // Step 2: Get the repeat count
        if (compressed[i] == '\0') {
            free(output);
            return NULL; // Missing repeat specifier
        }
        
        uint8_t repeat_count = (uint8_t)compressed[i];
        i++;

        // Ensure we have enough space in the output buffer
        if (output_length + repeat_count >= output_capacity) {
            size_t new_capacity = output_capacity * 2;
            while (output_length + repeat_count >= new_capacity) {
                new_capacity *= 2;
            }
            
            char* new_output = (char*)realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            
            output = new_output;
            output_capacity = new_capacity;
        }

        // Add the character to the output repeat_count times
        for (uint8_t j = 0; j < repeat_count; j++) {
            output[output_length++] = current_char;
        }
        output[output_length] = '\0';
    }

    // Resize the output to the exact size needed
    char* final_output = (char*)realloc(output, output_length + 1);
    if (final_output == NULL) {
        // If realloc fails, the original memory is still valid
        return output;
    }
    
    return final_output;
}
